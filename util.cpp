
#include "util.hpp"

// Measure the time it takes to access a block with virtual address addr.
inline CYCLES measure_one_block_access_time(ADDR_PTR addr)
{
	CYCLES cycles;

	asm volatile("mov %1, %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"mov %%eax, %%edi\n\t"
	"mov (%%r8), %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"sub %%edi, %%eax\n\t"
	: "=a"(cycles) /*output*/
	: "r"(addr)
	: "r8", "edi");	

	return cycles;
}

// Get current cpu cycle
CYCLES rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((CYCLES)hi << 32) | lo;
}

// Flush the memory address from all levels of CPU cache
volatile void clflush(void *p){
    asm volatile ("clflush (%0)" :: "r"(p));
}

void printAccessTime(ADDR_PTR addr) {
	printf("%u\n",  
		measure_one_block_access_time(addr));
}

void* largeMap(void *startAddr) {
	return map(startAddr, largeMapSize);
}

// Mmap a large region of memory. 
void* map(void *startAddr, size_t size) {
	void *ptr = mmap(
		(void *)((ADDR_PTR(startAddr) & ~(4096-1))), // align map to previous page boundary
		size,
		PROT_READ|PROT_EXEC|PROT_WRITE,
		MAP_ANONYMOUS|MAP_PRIVATE|MAP_FIXED,
		-1,
		0);

	if (ptr == MAP_FAILED) {
		printf("mmap failed: %s\n", strerror(errno));
		return NULL;
	}

	return ptr;
}

int unmap(void *addr) {
	int result = munmap(addr, largeMapSize);

	if (result == -1) {
		printf("munmap failed: %s\n", strerror(errno));
	}

	return result;
}