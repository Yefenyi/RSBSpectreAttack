
#include "util.hpp"

/* Measure the time it takes to access a block with virtual address addr. */
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

CYCLES rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((CYCLES)hi << 32) | lo;
}

volatile void clflush(void *p){
    asm volatile ("clflush (%0)" :: "r"(p));
}

void printAccessTime(ADDR_PTR addr) {
	printf("%u\n",  
		measure_one_block_access_time(addr));
}

void* copyFunction(ADDR_PTR startAddr, void *fnAddr) {
	void *ptr = mmap(
		(void *)startAddr,
		4096,
		PROT_READ|PROT_EXEC|PROT_WRITE,
		MAP_ANONYMOUS|MAP_PRIVATE,
		-1,
		0);

	if (ptr == MAP_FAILED) {
		printf("mmap failed: %s\n", strerror(errno));
		return NULL;
	}

	// printf("ptr is %p\n", ptr);

	ptr = memcpy(ptr, fnAddr, 1000);

	return ptr;
}

int unmap(ADDR_PTR addr) {
	int result = munmap((void *)(addr & ~(4096-1)), 4096);

	if (result == -1) {
		printf("munmap failed: %s\n", strerror(errno));
	}

	return result;
}