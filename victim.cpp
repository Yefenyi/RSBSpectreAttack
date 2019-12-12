#include "util.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int secretKey = 80;

char array[256*512];

volatile int foo(){
	__asm__(
		"cpuid;"
	);
	return 0;
}


int main(){
	string buffer;
	char temp;
	volatile int i = 0;

	ifstream secretFile("secret");
	if(secretFile.is_open()){
		getline(secretFile, buffer);
		secretFile.close();
	}

	string secret = buffer;
	temp &= secretKey; // vulneralbility

	unsigned int repeat = 1000, acc1 = 0, acc2 = 0, dummy;
	
	for(int i=0; i<repeat; i++){

		for(int i=0; i<16; i++){
			foo();
		}

		auto start = __rdtscp(&dummy);
		temp &= secretKey;
		auto end = __rdtscp(&dummy);
		acc1 += end-start;

		start = __rdtscp(&dummy);
		temp &= array[0];
		end = __rdtscp(&dummy);
		acc2 += end-start;

		clflush((void*)(&secretKey));
		clflush((void*)(&array[0]));
	}

	cout<<"average time on secret: "<<acc1/repeat<<endl;
	cout<<"average time on non-secret: "<<acc2/repeat<<endl;

	return 0;
}