#include "util.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sched.h>

using namespace std;

void callLoop();

int main(){
	ADDR_PTR addr = 0x0000555555565000;
	addr = (ADDR_PTR)copyFunction(addr, (void *)&callLoop);

	// callLoop();
	((void (*)())addr)();

	return 0;
}

// volatile void spacer() {
// 	asm(
// 		".rept 4880;"
// 		"nop;"
// 		".endr;"
// 	);
// }

void callLoop() {
	asm(
		"lbl:"
		"pop %rax;"
		"call lbl;"
	);
}