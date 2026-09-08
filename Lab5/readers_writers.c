#include 'user/user.h'
#include 'kernel/types.h'

struct shr{
    int val;
    int cnt;
}


int main(){
    struct shr *shr=(sturct shr*)shm_get();
    shr->val=0;
    shr->cnt=0;
    sem_init(0,1);
    sem_init(1,1);
    for(int i=0;i<3;i++){
        int p=fork();
        if(p==0){
            shr=(struct shr*)get_shm();
            for(int j=0;j<7;j++){
                sem_wait(0);
                shr->cnt++;
                if(shr->cnt==1){
                    sem_wait(1);
                }
                sem_signal(0);
                printf("Reader PID %d: shared_data=%d,active readers=%d\n",getpid(),shr->val,shr->cnt);
                sem_wait(0);
                shr->cnt--;
                if(shr->cnt==0){
                    sem_signal(1);
                }
                exit(0);
            }
        }
    }
    for(int i=0;i<2;i++){
        if(fork()==0){
            shr=(struct shr*)get_shm();
            for(int j=0;j<7;j++){
                sem_wait(1);
                s->val++;
                printf("Writer %d: %d\n",getpid(),s->val);
                sem_signal(1);
            }
            exit(0);
        }
    }
    for(int i=0;i<5;i++){
        wait(0);
    }
    exit(0);
}