#include "kernel/types.h"
#include "user/user.h"

#define n 5
#define m 3

int cu=0,cv=0;
int p[n+1];

int a[n+1][n+1];
void build_graph(int alloc[][m+1],int req[][m+1]){
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            if(req[i][j]){
                for(int k=1;k<=n;k++){
                    if(alloc[k][j]){
                        a[i][k]=1;
                    }
                }
            }
        }
    } 
}

void dfs(int u,int vis[]){
    vis[u]=1;
    for(int v=1;v<=n;v++){
        if(a[u][v]){
            if(vis[v]==0){
                p[v]=u;
                dfs(v,vis);
            }else if(vis[v]==1){
                cu=u;
                cv=v;
                return;
            }
        }
    }
    if(!cu&&!cv)return;
    vis[u]=2;
}

void print_cycle(){
    while(cu!=cv){
        printf("%d ",cu);
        cu=p[cu];
    }
    printf("%d\n",cv);
}

int main(){
    int alloc[n+1][m+1]={
        {0,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1},
        {0,0,0,0},
        {0,0,0,0}
    };

    int req[n+1][m+1]={
        {0,0,0,0},
        {0,0,1,0},
        {0,0,0,1},
        {0,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    };
    build_graph(alloc,req);
    int vis[n+1];
    for(int i=0;i<=n;i++)vis[i]=0;
    dfs(1,vis);
    if(cu==0&&cv==0){
        printf("No deadlock\n");
    }else{
        printf("Deadlock deteced!\n");
        print_cycle();
    }
    return 0;
}