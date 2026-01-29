# Memory-allocator
Custom memory allocator
Memory-efficient memory allocator that uses ~61% less heap memory than System malloc.
Includes benchmark program(bench.c) that when can be run with System malloc or custom malloc and prints amount of bytes heap as increased as well as time taken to allocate memory.
The benchmark program uses the rand function therefore to run, compile rand: gcc -c rand.o rand.c and compile my_malloc: gcc -c my_malloc.o my_malloc.c
To run the benchmark program using System malloc do gcc -o bench rand.o bench.c then ./bench and to run it using the custom malloc do gcc -o bench rand.o my_malloc.o bench.c then ./bench
