#include "kernel/types.h"
#include "user/user.h"

#define l1 0
#define l2 1

int main(int argc,char *argv[]){
    sem_init(l1,1);
    sem_init(l2,1);
    int p=fork();
    if(argc>1&&argv[1][0]=='1'){
        if(p==0){
            sem_wait(l2);
            printf("B takes l2\n");
            for(volatile int i=0;i<10000000;i++);
            printf("B waits for l1\n");
            sem_wait(l1);
            printf("B takes l1\n");
            exit(0);
        }
        sem_wait(l1);
        printf("A takes l1\n");
        for(volatile int i=0;i<10000000;i++);
        printf("A waits for l2\n");
        sem_wait(l2);
        printf("A takes l2\n");
        wait(0);
    }

    printf("resource ordering\n");
    sem_init(l1,1);
    sem_init(l2,1);
    p=fork();
    if(p==0){
        sem_wait(l1);
        printf("B takes l1\n");
        for(volatile int i=0;i<10000000;i++);
        sem_wait(l2);
        printf("B takes l2\n");
        printf("B completes work and releases resources.\n");
        sem_signal(l2);
        sem_signal(l1);
        exit(0);
    }
    sem_wait(l1);
    printf("B takes l1\n");
    for(volatile int i=0;i<10000000;i++);
    sem_wait(l2);
    printf("B takes l2\n");
    printf("B completes work and releases resources.\n");
    sem_signal(l2);
    sem_signal(l1);
    wait(0);
    exit(0);
}
