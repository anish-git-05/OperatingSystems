#include "types.h"
#include "user.h"

int main(){
    char *mem=sbrk(32*4096);
    int cnt=0;
    for(char *pt=mem;cnt<5;pt+=4096){
        *pt='t';
    }
    int pageFaults=getfaults();
    printf(1,"The number of page faults is %d\n",pageFaults);
    return 0;
}