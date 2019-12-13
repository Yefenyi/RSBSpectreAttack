#include "util.hpp"

using namespace std;

int main(){

	sem_t mutex; 
	// start child process
	pid_t pid = fork();
	// loads the entire array

	if(pid == 0){
		sem_wait(&mutex);
		printf("kid process started\n");
	}else if(pid>0){
		printf("start kid process\n");
		sem_post(&mutex);
		sleep(1);
		kill(pid, SIGKILL);
	}else{}

	return 0;
}
