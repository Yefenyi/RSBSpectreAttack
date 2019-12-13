#include "util.hpp"

using namespace std;

bool flag = false;
int temp;

void AddressAlignment(){
	__asm__(
		".rep 20143;"
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
	if(flag){
		throw ReturnException();
	}
	return 0;
}

int polluteLayer(){
	try{
		exceptionLayer();
	}catch(ReturnException& re){
		return 1;
	}
	return 0; 
}

int main(int argc, char** argv){
	if((argc==2) && (argv[1][0]=='1')){
		flag = true;
		cout<<"Start polluting RSB..."<<endl;
	}
	else if((argc==2) && (argv[1][0]=='0')){
		cout<<"Running without polluting RSB..."<<endl;
	}
	else{
		cout<<"Invalid argument"<<endl;
		return 1;
	}

	while(true){
		polluteLayer();
		temp+=1;
	}


	return 0;
}
