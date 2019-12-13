#include "util.hpp"
#include <iostream>
#include <fstream>

using namespace std;

char array[256*512];

volatile int foo(){
	return 0;
}

volatile int goo(){
	foo();
	int x = 5;
	return 0;
}


int main(){
	unsigned int dummy, acc=0;
	volatile int repeat = 10000;

	auto t1 = __rdtscp(&dummy);
	for(int i=0; i<repeat; i++){
		goo();
	}
	auto t2 = __rdtscp(&dummy);
	cout<<"avg time of goo() is:"<<(t2-t1)/repeat<<endl;

	return 0;
}