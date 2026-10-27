#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;
  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(n>0){
    myproc()->sz+=n;
    return addr;
  }
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_va2pa(void){
  int va;
  argint(0,&va);
  if(va<0){
    return -1;
  }
  return va2pa(va);
}

int sys_getmeminfo(void){
    char *addr;

    if (argptr(0, &addr, sizeof(struct meminfo)) < 0)
        return -1;

    struct meminfo *mi = (struct meminfo *)addr;

    int status = getmeminfo(mi);
    if (status < 0)
        return -1;

    cprintf("Total pages: %d\n"
            "Free pages: %d\n"
            "Calling process pages: %d\n"
            "Page table pages: %d\n",
            mi->total_pg, mi->free_pg, mi->proc_pg, mi->tab_pg);

    return 0;
}

int sys_vmprint(void){
  int status=vmprint();
  return 0;
}

int sys_getfaults(void){
  return myproc()->faults;
}