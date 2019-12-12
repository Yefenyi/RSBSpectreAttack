#include "util.hpp"
#include <iostream>
#include <fstream>

using namespace std;

char array[256*512];

volatile int foo(){
	return 0;
}

volatile void goo(){
	__asm__(
	".rept 452;"
	"nop;"
	".endr"
	);
}

volatile void gadget(){
	__asm__(
	"pop %rdi;"
	"pop %rdi;"	
	"pop %rdi;"
	"pop %rdi;"
	"nop;"
	"pop %rbp;"
	"clflush (%rsp);"
	"clflush (%rip);"
	"cpuid;"
	"retq;");
}

int speculative(){
	gadget();
	int paceholder = 0;
	return 0;
}

int main(){
	string buffer;
	char temp;
	unsigned int dummy;
	volatile int repeat = 1000;

	for(int i=0; i<8; i++){
		auto offset = (1<<12)*(i+8);
		temp &= array[offset];
	}

	for(int i=0; i<8; i++){
		auto offset = (1<<12)*(i+16);
		temp &= array[offset];
	}

	int sum = 0;
	
	speculative();

	auto t1 = __rdtscp(&dummy);
	for(int i=0; i<8; i++){
		auto offset = (1<<12)*(i+8);
		temp &= array[offset];
	}
	auto t2 = __rdtscp(&dummy);
	cout<<"time diff:"<< (t2-t1)/8<<endl;

	t1 = __rdtscp(&dummy);
	for(int i=0; i<8; i++){
		auto offset = (1<<12)*(i+16);
		temp &= array[offset];
	}
	t2 = __rdtscp(&dummy);
	cout<<"time diff:"<< (t2-t1)/8<<endl;


	return 0;
}