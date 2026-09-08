#include "kernel/types.h"
#include "user/user.h"
struct shared{
    int val;
    int flag[2];
    int turn;
};
int main(){
    struct shared *mem=(struct shared*)shm_get();
    mem->val=0;
    mem->flag[0]=0;
    mem->flag[1]=0;
    mem->turn=0;
    int p=fork();
    if(p==0){
        mem=(struct shared*)shm_get();
        int cur=0,oth=1;
        for(int i=0;i<11;i++){
            mem->flag[cur]=1;
            mem->turn=oth;
            while(mem->flag[oth]&&mem->turn==oth);
            (mem->val)++;
            printf("The process id is:%d and has the shared memory value as %d\n",getpid(),mem->val);
            mem->flag[cur]=0;
        }
    }else if(p>0){
        int cur=1,oth=0;
        for(int i=0;i<11;i++){
            mem->flag[cur]=1;
            mem->turn=oth;
            while(mem->flag[oth]&&mem->turn==oth);
            (mem->val)++;
            printf("The process id is:%d and has the shared memory value as %d\n",getpid(),mem->val);
            mem->flag[cur]=0;
        }
    }else{
        printf("The fork call failed!");
        return -1;
    }
    return 0;
}