#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

#define virt_addr 0x40000000
char *page_mem=0;

struct semaphore{
  int val;
  struct spinlock lock;
};

void down(struct semaphore *s)
{
  acquire(&s->lock);
  while(s->val==0){
    sleep_prepare(s);
    release(&s->lock);
    sleep();
    acquire(&s->lock);
  }
  s->val--;
  release(&s->lock);
}

void up(struct semaphore *s)
{
  acquire(&s->lock);
  s->val++;
  wakeup(s);
  release(&s->lock);
}

struct semaphore sem[3];
uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if (t == SBRK_EAGER || n < 0) {
    if (growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if (addr + n < addr)
      return -1;
    if (addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep_prepare(&ticks);
    release(&tickslock);
    sleep();
    acquire(&tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 sys_shm_get(void){
  struct proc *p=myproc();
  if(page_mem==0){
    page_mem=kalloc();
    if(page_mem==0){
      return -1;
    }
    memset(page_mem,0,PGSIZE);
  }
  uint64 virtAddr=virt_addr;
  int tmp=mappages(p->pagetable,virtAddr,PGSIZE,(uint64)page_mem,PTE_R|PTE_W|PTE_U);
  if(tmp<0){
    return -1;
  }
  return virtAddr;
}

uint64 sys_sem_init(void){
  int id,value;
  argint(0,&id);
  argint(1,&value);
  sem[id].val=value;
  initlock(&sem[id].lock,"semie :)");
  return 0;
}

uint64 sys_sem_wait(void){
  int id;
  argint(0,&id);
  down(&sem[id]);
  return 0;
}

uint64 sys_sem_signal(void){
  int id;
  argint(0,&id);
  up(&sem[id]);
  return 0;
}

uint64 sys_clone(void){
  uint64 fcn,arg,stack;
  argaddr(0,&fcn);
  argaddr(1,&arg);
  argaddr(2,&stack);
  return clone((void*)fcn,(void*)arg,(void*)stack);
}