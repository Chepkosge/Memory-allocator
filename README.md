# Memory-allocator

Custom memory allocator and benchmark.

This repository contains:
- A simple custom malloc implementation (`my_malloc.c`).
- A benchmark program (`bench.c`) and a small random helper (`rand.c`) that measures heap growth and allocation time when using the system `malloc` vs the custom allocator.

Quick notes
- The benchmark prints the number of bytes the heap increased by and the elapsed time for the allocation workload.
- In an example run (see below) the custom allocator used ~61% less heap compared to the system `malloc`. See the "Example benchmark output" section.

Prerequisites
- GCC (or another C compiler)
- Make (optional)
- Unix-like environment

Build and run

1) Compile helper/object files
- Compile rand.c and other sources into objects:
  gcc -c -O2 rand.c -o rand.o
  gcc -c -O2 bench.c -o bench.o
  (If building the custom allocator variant, also compile my_malloc.c)
  gcc -c -O2 my_malloc.c -o my_malloc.o

2) Build and run using system malloc
- Build:
  gcc -O2 -o bench-system rand.o bench.o
- Run:
  ./bench-system

3) Build and run using the custom allocator (linking it into the binary)
- Build:
  gcc -O2 -o bench-custom rand.o my_malloc.o bench.o
- Run:
  ./bench-custom

(Alternatively you can compile and link in one step: `gcc -O2 -o bench rand.c bench.c` for system malloc, or `gcc -O2 -o bench rand.c my_malloc.c bench.c` for custom allocator.)

Example benchmark output
- These are example results from a single run on a test machine:

System malloc:  
Heap increased by: 5,120,000 bytes  
Elapsed time: 0.220 seconds

Custom allocator:  
Heap increased by: 1,996,800 bytes  
Elapsed time: 0.185 seconds

Reduction in heap usage: ~61.0%

Notes:
- Results vary across machines, compilers, and runtime conditions. Run multiple trials and take the median for more stable comparisons.

Short allocator design write-up

High-level goals
- Reduce heap fragmentation and overall heap growth for the benchmark workload.
- Provide a simple, educational allocator implementation (not production-grade).

Key data structures
- Block header: each allocated/free block includes a small header stored just before the payload. The header stores block size and pointers used for free-list linking.
- Free lists / bins: free blocks are organized into size classes (bins). Small-to-medium sizes use segregated free lists to speed allocation and reduce fragmentation.
- Splitting: when a free block is larger than requested and can be split without producing a too-small remainder, the block is split into an allocated piece and a smaller free block (returned to an appropriate bin).
- Coalescing: on free, adjacent free blocks are coalesced (merged) to form larger free blocks. Coalescing reduces fragmentation and the need to request more memory from the OS.
- Allocation policy: the allocator uses a first-fit search within an appropriate bin (or the next larger bin) to find a free block. This keeps search time short while leveraging the size-segregated structure.
- Growth strategy: when no suitable free block exists, the allocator expands the heap using sbrk() (or an equivalent mechanism) to obtain more memory. Large allocations could be routed through mmap() (not required for the benchmark).

Metadata and alignment
- Blocks are aligned to at least 8 bytes (or platform word size) to satisfy typical alignment requirements.
- The header size is accounted for when choosing split thresholds.

Thread-safety and limitations
- The current implementation is not thread-safe (no locks). Concurrent allocations/frees from multiple threads can corrupt structures.
- The allocator is simplified for clarity and the benchmark workload; it lacks advanced features such as per-thread caches (tcmalloc-like), robust mmap handling for huge allocations, or comprehensive error handling.
- Not hardened against malicious/free-list-corruption inputs—use with caution.
