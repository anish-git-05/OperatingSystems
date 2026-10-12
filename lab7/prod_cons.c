#include "kernel/types.h"
#include "user/user.h"

const int EMPTY=0,FULL=1,MUTEX=2;
struct shared{
    int buffer[20];
    int in;
    int out;
};

int main(){
    struct shared *mem;
    mem=(struct shared*)shm_get();
    mem->in=0;
    mem->out=0;
    sem_init(EMPTY,20);
    sem_init(FULL,0);
    sem_init(MUTEX,1);
    int p=fork();
    if(p==0){
        mem=(struct shared*)shm_get();
        for(int i=0;i<20;i++){
            sem_wait(FULL);
            sem_wait(MUTEX);
            int a=mem->buffer[mem->out];
            mem->out=(mem->out+1)%20;
            printf("consumer %d: %d",getpid(),a);
        }
        exit(0); 
    }
    for(int i=0;i<20;i++){
        sem_wait(EMPTY);
        sem_wait(MUTEX);
        mem->buffer[mem->in]=i+1;
        mem->in=(mem->in+1)%20;
        sem_signal(MUTEX);
        sem_signal(FULL);
        printf("producer %d: %d",getpid(),i+1);
    }
    wait(0);
    exit(0);
}