#include "util.hpp"
#include <iostream>
#include <fstream>
#include <sys/mman.h>
#include <string.h>

using namespace std;

char array[256*256];

volatile void spacer() {
	asm(
		// ".rept 2235;"
		".rept 2215;"
		// ".rept 2062;"
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
	// printf("polluting rsb\n");
	// gadget();

	ADDR_PTR start = 0x0000555555556000;

	void *ptr = mmap((void *)start,
		4096,
		PROT_READ|PROT_EXEC|PROT_WRITE,
		MAP_ANONYMOUS|MAP_SHARED,
		-1,
		0);

	if (ptr == MAP_FAILED) {
		printf("mmap failed: %s\n", strerror(errno));
		return 0;
	}

	printf("ptr is %p\n", ptr);

	ptr = memcpy(ptr, (void *)&gadget, 32);

	((int (*)())ptr)();

	return 0;
}