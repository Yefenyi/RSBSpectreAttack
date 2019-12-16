#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <inttypes.h>
#include <time.h>
#include <iostream>
#include <x86intrin.h>
#include <sys/mman.h>
#include <string.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t 
#define CYCLES uint32_t

#define largeMapSize 1<<20
// #define largeMapSize 1<<19

CYCLES measure_one_block_access_time(ADDR_PTR addr);
CYCLES rdtsc();
volatile void clflush(void *p);
void printAccessTime(ADDR_PTR addr);
void* largeMap(void *startAddr);
void* map(void *startAddr, size_t size);
int unmap(void *startAddr);

#endif