#include "util.hpp"
#include <iostream>
#include <fstream>

using namespace std;

char array[256*256];

volatile void spacer() {
	asm(
		".rept 2189;"
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

int main(){
	printf("Polluting RSB\n");
	gadget();
}