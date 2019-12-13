#include "util.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int secretKey = 80;

int nonSecretKey = 0;

char array[256*512];

void AddressAlignment(){
	__asm__(
		".rep 10000;"
		"cpuid;"
		".endr;"
	);
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
	temp &= array[secretKey*512]; // vulneralbility

	unsigned int repeat = 100, acc1 = 0, acc2 = 0, dummy;
	
	for(int i=0; i<repeat; i++){

		for(int i=0; i<4; i++){
			AddressAlignment();
			temp &= i;
		}

		unsigned int start, end;

		start = __rdtscp(&dummy);
		temp &= array[secretKey*512];
		end = __rdtscp(&dummy);
		acc1 += end-start;

		start = __rdtscp(&dummy);
		temp &= array[nonSecretKey*512];
		end = __rdtscp(&dummy);
		acc2 += end-start;


		clflush((void*)(&array[secretKey*512]));
		clflush((void*)(&array[nonSecretKey*512]));
	}

	cout<<"average time on secret: "<<acc1/repeat<<endl;
	cout<<"average time on non-secret: "<<acc2/repeat<<endl;

	return 0;
}