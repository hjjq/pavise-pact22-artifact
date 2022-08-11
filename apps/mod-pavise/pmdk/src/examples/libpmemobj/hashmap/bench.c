/*
 * Copyright (c) 2017 The National University of Singapore.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <x86intrin.h>

#include "../map/map.h"
#include "hashmap_atomic.h"
#include "hashmap_tx.h"
#include "hashmap_notx.h"

#define OBJ_TYPE_NUM 1

#define ATOMIC_MAP    1
#define TX_MAP        2
#define NOTX_MAP      3

#define INSERT 1
#define UPDATE 2

POBJ_LAYOUT_BEGIN(map);
POBJ_LAYOUT_ROOT(map, struct root);
POBJ_LAYOUT_END(map);

struct root {
	TOID(struct map) map;
};

static TOID(struct root) root;

/*
 * Get the time in milliseconds.
 */
static uint64_t get_time(void)
{
    // Linux:
    struct timespec ts;
    unsigned tick = 0;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    tick  = ts.tv_nsec / 1000000;
    tick += ts.tv_sec * 1000;
    return tick;
}

/*
 * Run the benchmark.
 */
void do_bench(FILE *stream, int impl, int op, size_t start, size_t end,
              size_t tick, char *path)
{
    for (size_t n = start; n <= end; n += tick)
    {
        fprintf(stdout, "Using pmem backing file:%s\n", path);
        PMEMobjpool *pop = pmemobj_create(path, POBJ_LAYOUT_NAME(map),
                PMEMOBJ_MIN_POOL*100, 0666);
		if (pop == NULL) {
            fprintf(stderr, "failed to create pool: %s\n",
                    pmemobj_errormsg());
            return;
        }
            
        root = POBJ_ROOT(pop, struct root);

        PMEMoid value_ptr; 

        int count_expected = 0;

        fprintf(stream, "\n%zu ", n);
            
        switch (impl)
        {
            case ATOMIC_MAP:
            {
                TOID(struct hashmap_atomic) *map = (TOID(struct hashmap_atomic) *) &D_RW(root)->map;
                if (hm_atomic_create(pop, map, NULL)) {
                    perror("map_new");
                    return;
                } 
                size_t t0 = get_time();
                if (op == INSERT) {
                    for (int i = 0; (unsigned int) i < n; i++) {
                        pmemobj_zalloc(pop, &value_ptr, sizeof(int), OBJ_TYPE_NUM);
                        hm_atomic_insert(pop, *map, i, value_ptr); 
                    }
                    count_expected = n;
                } else if (op == UPDATE) {
                    // Create value once, and then update same value.
                    pmemobj_zalloc(pop, &value_ptr, sizeof(int), OBJ_TYPE_NUM);
                    for (int i = 0; (unsigned int) i < n; i++) {
                        // TODO: Update value.
                        hm_atomic_insert(pop, *map, 0, value_ptr); 
                    }
                    count_expected = 1;
                }
                size_t t1 = get_time();
                fprintf(stream, "%zu\n", t1 - t0);
                assert (hm_atomic_count(pop, *map) == count_expected);
                break;
            }
            case TX_MAP:
            {
                TOID(struct hashmap_tx) *map = (TOID(struct hashmap_tx) *) &D_RW(root)->map;
                if (hm_tx_create(pop, map, NULL)) {
                    perror("map_new");
                    return;
                } 
                size_t t0 = get_time();
                if (op == INSERT) {
                    for (int i = 0; (unsigned int) i < n; i++) {
                        // We allocate explicitly here as insert
                        // needs pointer to allocated value.
                        // Zalloc zeroes out location, which acts as dummy of 
                        // actually updating location with real value.
                        // We want to actually use alloc and then write in real
                        // value, but hard to do as it takes parameter contructor.
                        pmemobj_zalloc(pop, &value_ptr, sizeof(int), OBJ_TYPE_NUM);
                        hm_tx_insert(pop, *map, i, value_ptr); 
                    } 
                    count_expected = n;
                } else if (op == UPDATE) {
                    // Create value once, and then update same value.
		    pmemobj_zalloc(pop, &value_ptr, sizeof(int), OBJ_TYPE_NUM);
                    for (int i = 0; (unsigned int) i < n; i++) {
                        // TODO: Update value.
                        hm_tx_insert(pop, *map, 0, value_ptr); 
                    } 
                    count_expected = 1;
                }
                size_t t1 = get_time();
                fprintf(stream, "%zu\n", t1 - t0);
                assert (hm_tx_count(pop, *map) == count_expected);
                break;
            }
            case NOTX_MAP:
            {
                TOID(struct hashmap_notx) *map = (TOID(struct hashmap_notx) *) &D_RW(root)->map;
                if (hm_notx_create(pop, map, NULL)) {
                    perror("map_new");
                    return;
                } 
                size_t t0 = get_time();
                if (op == INSERT) {
                    for (int i = 0; (unsigned int) i < n; i++) {
                        pmemobj_zalloc(pop, &value_ptr, sizeof(int), OBJ_TYPE_NUM);
                        hm_notx_insert(pop, *map, i, value_ptr); 
                    }
                    count_expected = n;
                } else if (op == UPDATE) {
                    // Create value once, and then update same value.
                    pmemobj_zalloc(pop, &value_ptr, sizeof(int), OBJ_TYPE_NUM);
                    for (int i = 0; (unsigned int) i < n; i++) {
                        *((int*)pmemobj_direct(value_ptr)) = i; 
                        hm_notx_insert(pop, *map, 0, value_ptr); 
                    }
                    count_expected = 1;
		    fprintf(stdout, "Value:%d\n", *((int*)pmemobj_direct(hm_notx_get(pop, *map, 0))));
                }
                size_t t1 = get_time();
                fprintf(stream, "%zu\n", t1 - t0);
                assert (hm_notx_count(pop, *map) == count_expected);
                break;
            }
            default:
            fprintf(stderr, "error: unknown impl (%d)\n", impl);
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 7)
    {
        fprintf(stderr, "usage: %s impl op start end tick backing-file\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
    int impl = 0;
    if (strcmp(argv[1], "map_atomic") == 0)
        impl = ATOMIC_MAP;
    else if (strcmp(argv[1], "map_tx") == 0)
        impl = TX_MAP;
    else if (strcmp(argv[1], "map_notx") == 0)
        impl = NOTX_MAP;
    else
    {
        fprintf(stderr, "error: bad impl \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    int op = 0;
    if (strcmp(argv[2], "insert") == 0)
        op = INSERT;
    else if (strcmp(argv[2], "update") == 0)
        op = UPDATE;
    else {
        fprintf(stderr, "error: bad operation \"%s\"\n", argv[2]);
        exit(EXIT_FAILURE);
    }



    size_t start = atoll(argv[3]);
    size_t end   = atoll(argv[4]);
    size_t tick  = atoll(argv[5]);
    char* file_path = argv[6];
    do_bench(stdout, impl, op, start, end, tick, file_path);

    return 0;
}
