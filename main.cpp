#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include "util.hpp"

using namespace std; 
  
int main() 
{ 
   // ftok to generate unique key 
    // key_t key = ftok("shmfile",65); 
  
    // // shmget returns an identifier in shmid 
    // int shmid = shmget(key,1024,0666|IPC_CREAT); 

    // // shmat to attach to shared memory 
    // char* addr = (char*) shmat(shmid,(void*)0,0); 
      
    // unsigned int index = 10, repeat = 1000, dummy;
    // char temp;
    // for(int i=0;;i++){
    // 	temp&=addr[i];
    // 	printf("%d\n",i);
    // }

    // shmdt(addr); 
    
    // // destroy the shared memory 
    // shmctl(shmid,IPC_RMID,NULL); 

    const int SIZE = 1<<20; 
  
    /* name of the shared memory object */
    const char* name = "OS"; 
    char temp;
  
    /* shared memory file descriptor */
    int shm_fd; 
  
    /* pointer to shared memory obect */
    char* ptr; 
  
    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666); 
  
    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE); 
  
    /* memory map the shared memory object */
    ptr = (char*)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0); 

    while(1) {
        clflush((void *)(&ptr[0]));
    }
     
    return 0;
} 