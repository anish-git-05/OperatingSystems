// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"
#include "proc.h"

struct meminfo{
  int total_pg;
  int free_pg;
  int proc_pg;
  int tab_pg;
};

int count_free=0;
void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
} kmem;

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kfree(p);
}
//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
  count_free++;
  if(kmem.use_lock)
    release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  count_free--;
  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}

int getmeminfo(struct meminfo *mi){
  struct proc *p=myproc();
  mi->total_pg=PHYSTOP/PGSIZE;
  mi->free_pg=count_free;
  mi->proc_pg=myproc()->sz/PGSIZE;
  for(int i=0;i<1024;i++){
    if(p->pgdir[i]&PTE_P){
      mi->tab_pg++;
    }
  }
  return 0;
}

int vmprint(void){
  struct proc *p=myproc();
  pde_t *pgdir=p->pgdir;
  int kernel=0;
  for(int i=0;i<NPDENTRIES;i++){
    pde_t pde=pgdir[i];
    if ((pde & PTE_P) == 0)continue;
    pte_t *pt=(pte_t*)P2V(PTE_ADDR(pde));
    int shown=0;
    for(int j=0;j<NPTENTRIES;j++){
      pte_t pte=pt[j];
      if ((pte&PTE_P) == 0)continue;
      uint va=((uint)i<<PDXSHIFT)|((uint)j<<PTXSHIFT);
      if(va>=KERNBASE){
        kernel++;
        continue;
      }
      if(!shown){
        cprintf("PDE[%d]\n",i);
        shown=1;
      }
      cprintf("  PTE[%d] VA=%x PA=%x P=%d W=%d U=%d\n",j,va,PTE_ADDR(pte),(pte&PTE_P)!=0,(pte&PTE_W)!=0,(pte&PTE_U)!=0);
    }
  }
  cprintf("Kernel entries: %d\n",kernel);
  return 0;
}