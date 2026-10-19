# xv6 Memory Management

## Problem 1: Virtual Address to Physical Address Translation

The function `vm2pa` is written in `vm.c`. It basically maps a logical (virtual) address to a physical address using the standard 10/10/12 bits two-level division logic.

The 32-bit virtual address is divided into three parts: 10 bits for the page directory index, 10 bits for the page table index and 12 bits for the page offset. Using these indices, the function accesses the corresponding page directory and page table entries to obtain the physical address of the frame. The page offset is then added to the physical frame address to get the final physical address.

## Problem 2: Memory Information

The function `meminfo` is written in `kalloc.c`. It calculates the number of total pages, page table pages, process pages and free pages.

This function helps in understanding how physical memory is being used by the kernel and user processes. It provides information about the total available memory pages and how these pages are distributed among free memory, process memory and page tables.

## Problem 3: Page Table Printing

The function `vmprint` is also written in `kalloc.c`. It prints the page directory and page table entries.

The function traverses the page directory and the corresponding page tables. For each valid page table entry, it prints the entry index, virtual address, physical frame address and the flags (P, W, U). Indentation is used to show the two levels of the page table clearly.

Only entries below `KERNBASE` are printed, followed by a one-line summary of the number of kernel entries. This helps in understanding how virtual addresses are mapped to physical memory and how the page tables are structured.

## Problem 4: Lazy Allocation

This problem handles demand paging and is implemented in `sysproc.c` and `trap.c`.

When the heap memory has to be increased using `sbrk()`, the kernel does not directly allocate physical pages. Instead, it only increases the process memory size. The physical pages are allocated only when they are actually needed, that is, when the process accesses those virtual addresses.

When a page fault occurs, the trap handler checks whether the fault is caused by a valid access to an unallocated part of the heap. If it is a valid access, a physical page is allocated, cleared and mapped to the corresponding virtual address. This avoids allocating physical memory for pages that are never used by the process.

The `lazytest.c` program tests this behaviour by requesting a large heap region, touching only a few widely spaced pages and checking the memory information before and after. It also tests the behaviour of `fork()`, `exit()` and a system call that uses an untouched buffer.