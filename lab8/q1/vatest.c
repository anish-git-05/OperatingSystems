#include "types.h"
#include "user.h"

int g_var=23;
int main(){
    int c=fork();
    if(c==0){
    printf(1,"For child process\n");
    int l_var=20;
    int *pt=(int*)malloc(20*sizeof(int));
    int pa_g_var=va2pa((int)&g_var);
    int pa_l_var=va2pa((int)&l_var);
    int pa_pt=va2pa((int)pt);
    printf(1,"Virtual address of local variable is &d",&l_var);
    printf(1,"Virtual address of global variable is &d\n",&g_var);
    printf(1,"Virtual address of malloc block is &d\n",pt);
    printf(1,"Physical address of local variable is &d\n",&pa_l_var);
    printf(1,"Physical address of global variable is &d\n",&pa_g_var);
    printf(1,"Physial address of malloc block is &d",pa_pt);
    }
    else if(c>0){
        printf(1,"For parent process\n");
        int l_var=20;
        int *pt=(int*)malloc(20*sizeof(int));
        int pa_g_var=va2pa((int)&g_var);
        int pa_l_var=va2pa((int)&l_var);
        int pa_pt=va2pa((int)pt);
        printf(1,"Virtual address of local variable is &d",&l_var);
        printf(1,"Virtual address of global variable is &d\n",&g_var);
        printf(1,"Virtual address of malloc block is &d\n",pt);
        printf(1,"Physical address of local variable is &d\n",&pa_l_var);
        printf(1,"Physical address of global variable is &d\n",&pa_g_var);
        printf(1,"Physial address of malloc block is &d",pa_pt);
        wait();
        exit();
    }else{
        exit();
    }
}