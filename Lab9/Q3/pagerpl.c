#include "types.h"
#include "user.h"

int main(){
    int n;
    int m=3;
    int refString[100];
    char buf[32];
    int hitsFCFS=0;
    int hitsLRU=0;
    gets(buf,sizeof(buf));
    n=atoi(buf);
    for(int i=0;i<m;i++){
        gets(buf,sizeof(buf));
        refString[i]=atoi(buf);
    }
    // First we will do FIFO
    int frames[100];
    for(int i=0;i<100;i++)frames[i]=0;
    int front=1,back=1;
    for(int i=0;i<m;i++){
        int reqP=refString[i];
        int f=0;
        int foundFrame=0;
        for(int j=0;j<n;j++){
            if(frames[j]==reqP){
                f=1;
                foundFrame=back-front+1;
                break;
            }
        }
        if(f==1){
            printf(1,"Page hit\nFound in frame %d",foundFrame);
            hitsFCFS++;
        }else{
            printf(1,"Page fault\n");
            if(back-front+1==n){
                front++;
            }
            frames[back++]=reqP;
            printf(1,"The new page is in frame %d",back-front+1);
        }
    }
    //Now we will do LRU
    int time=0;
    int used[100];
     for(int i=0;i<100;i++)frames[i]=0;
    front=0;
    back=0;
    for(int i=0;i<m;i++){
        int reqP=refString[i];
        int f=0;
        int foundFrame=0;
        for(int j=0;j<n;j++){
            if(frames[j]==reqP){
                f=1;
                foundFrame=back-front+1;
                break;
            }
        }
        if(f==1){
            printf(1,"Page hit\nFound in frame %d",foundFrame);
            used[foundFrame]=time;
            hitsLRU++;
        }else{
            printf(1,"Page fault\n");
            if(back-front+1<n){
                printf(1,"The new page is in frame %d",back);
                back++;
                continue;
            }
            int mn=10000000;
            int frameRep=0;
            for(int j=1;j<=n;j++){
                if(used[j]<mn){
                    mn=used[j];
                    frameRep=j;
                    break;
                }
            }
            frames[frameRep]=reqP;
            used[frameRep]=time;
            printf(1,"The new frame alloted is %d",frameRep);
        }
        time++;
    }
    printf(1,"FCFC hit ratio is %d %",(hitsFCFS*100)/m);
    printf(1,"LRU hit ratio is %d %",(hitsLRU*100)/m);
    return 0;
}