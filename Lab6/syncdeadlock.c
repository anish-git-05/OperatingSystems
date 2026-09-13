#include "kernel/types.h"
#include "user/user.h"


#define P 0
#define S 1
#define D 2

void solve(int id,int a,int b){
    printf("Proc%d requests res%d\n",id,a);
    sem_wait(a);
    printf("Proc%d takes res%d\n",id,a);
    printf("Proc%d requests res%d\n",id,b);
    sem_wait(b);
    printf("Proc%d takes res%d\n",id,b);
    printf("Proc%d starts work\n",id);
    for(volatile int i=0;i<1000000;i++);
    sem_signal(b);
    sem_signal(a);
    printf("Proc%d releases resources!!",id);
}

int main(){
    sem_init(P,2);
    sem_init(S,1);
    sem_init(D,2);
    int a[]={P,S,P,P,S};
    int b[]={S,D,D,S,D};
    for(int i=0;i<5;i++){
        if(fork()==0){
            for(int j=0;j<3;j++){
                solve(i,a[i],b[i]);
            }
            exit(0);
        }
    }
    for(int i=0;i<5;i++){
        wait(0);
    }
    exit(0);
}