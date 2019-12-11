#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <inttypes.h>
#include <time.h>
#include <iostream>
#include <x86intrin.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t 
#define CYCLES uint32_t

CYCLES measure_one_block_access_time(ADDR_PTR addr);
CYCLES rdtsc();
void clflush(volatile void *p);

#endif