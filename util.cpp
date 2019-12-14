
#include "util.hpp"

/* Measure the time it takes to access a block with virtual address addr. */
inline CYCLES measure_one_block_access_time(ADDR_PTR addr)
{
	CYCLES cycles;

	asm volatile("mov %1, %%r8\n\t"
	"mfence\n\t"
	"rdtsc\n\t"
	"mov %%eax, %%edi\n\t"
	"mov (%%r8), %%r8\n\t"
	"mfence\n\t"
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