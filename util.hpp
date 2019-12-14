#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <inttypes.h>
#include <time.h>
#include <iostream>
#include <x86intrin.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t 
#define CYCLES uint32_t

#define lowerBound 0
#define upperBound 9
#define offset 1024
#define sleepTime 0.5
#define nAttackRepeat 20
#define INT_MAX 2147483647

CYCLES measure_one_block_access_time(ADDR_PTR addr);
CYCLES rdtsc();
volatile void clflush(void *p);
void printAccessTime(ADDR_PTR addr);

#endif