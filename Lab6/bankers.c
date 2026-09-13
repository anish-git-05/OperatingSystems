#include "kernel/types.h"
#include "user/user.h"

#define n 5
#define m 3

int Allocation[n][m]={
    {1,0,0},
    {0,1,0},
    {1,0,1},
    {0,0,1},
    {0,0,0}
};

int Max[n][m]={
    {1,1,0},
    {0,1,1},
    {1,1,1},
    {0,0,1},
    {1,1,0}
};

int Available[m]={1,0,1};

int existsAllocation(int Available[m],int Allocation[][m],int Max[][m]){

    int work[m];
    int finish[n]={0};

    for(int j=0;j<m;j++)
        work[j]=Available[j];

    for(int k=0;k<n;k++){

        for(int i=0;i<n;i++){

            if(finish[i])
                continue;

            int f=1;

            for(int j=0;j<m;j++){

                if(Max[i][j]-Allocation[i][j]>work[j]){

                    f=0;
                    break;

                }

            }

            if(f){

                for(int j=0;j<m;j++)
                    work[j]+=Allocation[i][j];

                finish[i]=1;

            }

        }

    }

    for(int i=0;i<n;i++)
        if(!finish[i])
            return 0;

    return 1;

}

int resourceRequest(int Available[m],int Allocation[][m],int Max[][m],int Pi,int resID){

    if(Max[Pi][resID]-Allocation[Pi][resID]<=0||Available[resID]<=0){

        printf("Request denied!");

        return 0;

    }

    Available[resID]-=1;
    Allocation[Pi][resID]+=1;

    if(existsAllocation(Available,Allocation,Max)){

        printf("Request granted!");

        return 1;

    }else{

        Available[resID]+=1;
        Allocation[Pi][resID]-=1;

        printf("Request denied!");

        return 0;

    }

}

int main(){
    resourceRequest(Available,Allocation,Max,0,1);
    exit(0);

}