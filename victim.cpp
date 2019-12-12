#include "util.hpp"
#include <iostream>
#include <fstream>
#define secret 1232

using namespace std;

char array[256*256];

volatile int foo(){
	return 0;
}

void exploit() { // this should be at the same VA as attacker:gadget()
	volatile int temp = array[secret];
}

inline void timeAndFlush(ADDR_PTR addr) {
	printAccessTime(addr);
	clflush((void *)addr);
}

void trickMe() {
	while(1) {
		foo();
		timeAndFlush((ADDR_PTR)&array[secret]);
	}
}

int main(){
	trickMe();

	// string buffer;
	// char temp;
	// volatile int i = 0;

	// ifstream secretFile("secret");
	// if(secretFile.is_open()){
	// 	getline(secretFile, buffer);
	// 	secretFile.close();
	// }

	// string secret = buffer;
	// temp &= array[secret[i]*256]; // vulneralbility

	// while(true){
	// 	foo();
	// 	measureAccessTime();
	// }

	return 0;
}