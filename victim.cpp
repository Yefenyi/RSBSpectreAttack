#include "util.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sched.h>

using namespace std;

void callLoop() {
	asm(
		"lbl:"
		"pop %rax;"
		"call lbl;"
	);
}

int main(){
	void *requestedAddr = (void *)0x0000555555565000;
	printf("requesting memory at %p\n", requestedAddr);
	void *newFunc = map(requestedAddr, 1024);
	printf("starting address is %p\n", newFunc);
	memcpy(newFunc, (void *)&callLoop, 1024);

	// callLoop();
	((void (*)())newFunc)();

	return 0;
}

// volatile void spacer() {
// 	asm(
// 		".rept 4880;"
// 		"nop;"
// 		".endr;"
// 	);
// }