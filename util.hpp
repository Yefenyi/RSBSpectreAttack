#include <stdio.h>

// You may use memory allocators and helper functions 
// (e.g., rand()).  You may not use system().
#include <stdlib.h>
#include <stdint.h>
#include <cstdlib>
#include <inttypes.h>
#include <time.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t 
#define CYCLES uint32_t

CYCLES measure_one_block_access_time(ADDR_PTR addr);
CYCLES rdtsc();
void clflush(volatile void *p);

#endif