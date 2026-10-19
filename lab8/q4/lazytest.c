#include "types.h"
#include "user.h"

struct meminfo{
  int total_pg;
  int free_pg;
  int proc_pg;
  int tab_pg;
};

int main(void){
  struct meminfo *mi=(struct meminfo *)malloc(sizeof(struct meminfo));
  char *mem;
  int pid;
  printf(1,"Before sbrk:\n");
  int st=getmeminfo(mi);
  mem=sbrk(32 * 1024 * 1024);
  if(mem==(char*)-1){
    printf(1,"sbrk failed\n");
    exit();
  }
  printf(1,"After sbrk:\n");
  st=getmeminfo(mi);
  for(int i=0;i<8;i++){
    mem[i * 4 * 1024 * 1024]='a'+i;
  }
  printf(1,"After touching pages:\n");
  st=getmeminfo(mi);
  if(write(1,mem + 2 * 1024 * 1024,1)<0)
    printf(1,"write failed\n");
  else
    printf(1,"write on untouched buffer succeeded\n");
  pid=fork();
  if(pid==0){
    mem[16 * 1024 * 1024]='x';
    printf(1,"Child touched an untouched page\n");
    exit();
  }
  if(pid>0)
    wait();
  else
    printf(1,"fork failed\n");
  printf(1,"After fork and child exit:\n");
  st=getmeminfo(mi);
  exit();
}