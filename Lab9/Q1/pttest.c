#include "types.h"
#include "user.h"

int main(){
    sbrk(4*4096);
    for(int i=0;i<4;i++){
        if(vmprint()<0){
            printf(1,"error\n");
        };
    }
    return 0;
}