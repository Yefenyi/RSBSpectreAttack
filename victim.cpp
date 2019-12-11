#include "util.hpp"
#include <iostream>
#include <fstream>

using namespace std;

char array[256*256];

volatile int foo(){
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
	temp &= array[secret[i]*256]; // vulneralbility

	while(true){
		foo();
	}

	return 0;
}