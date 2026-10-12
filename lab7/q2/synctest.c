#include "kernel/types.h"
#include "user/user.h"

volatile int counter = 0;

#define MUTEX 1
seminit(MUTEX,1);
void increment(void *arg){
    int i;
    for(i=0;i<10000;i++) {
        wait(MUTEX);
        counter++;
        signal(MUTEX);
    }
    exit();
}

int main(){
    int i;
    int tid;
    char *stack;
    printf(1,"Starting thread test...\n");
    for (i=0;i<3;i++) {
        stack = malloc(4096);
        if (stack==0) {
            printf(1, "Failed to allocate stack\n");
            exit();
        }
        tid = clone(increment,0,stack);
        if (tid<0) {
            printf(1, "clone failed\n");
            free(stack);
            exit();
        }
        printf(1, "Created thread %d\n", tid);
    }

    for (i=0;i<3; i++) {
        tid = join();
        if (tid<0) {
            printf(1, "join failed\n");
            exit();
        }
        printf(1, "Joined thread %d\n", tid);
    }

    printf(1, "Final counter value: %d\n", counter);
    printf(1, "Expected counter value: %d\n",3*10000);
    exit();
}
