/*
 *  Fixed-size memory allocator
 *  Based on challoc: https://github.com/dhamidi/challoc
 *  (C) Copyright 2011, 2012, 2013, Dario Hamidi <dario.hamidi@gmail.com>
 *  Modified for persistent memory
 */

#ifndef PAVISE_ALLOC_H
#define PAVISE_ALLOC_H

#include <pthread.h>
#include <x86intrin.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <assert.h>
#include <vector>
#define force_inline __attribute__((always_inline)) inline

//#define ENABLE_RECOVERY // enable protection of data used in recovery only
#define ENABLE_PERSIST

#define VERIFY_PERIOD 10000

#ifdef ENABLE_PERSIST

#define clwb_direct(a) _mm_clwb(a)
#define PSFENCE() _mm_sfence()
#define pavise_setvalid(hdr,val) \
_mm_stream_si32(&hdr->valid, val); \
PSFENCE(); 

#else

#define clwb_direct(a)
#define PSFENCE()
#define pavise_setvalid(hdr,val)

#endif

#define PAGESIZE 4096ULL
#define PCACHELINE_SIZE 64
#define PAVISE_LAYOUT_HDR_SIZE sizeof(struct pavise_layout_header)

#ifdef __cplusplus
extern "C" {
#endif

#include "pavise_memcpy_nt_sse2.h"

// header that contains info about layout of Pavise pool
struct pavise_layout_header {
    int valid;
    size_t hdr_size; // size of header region (note: not the size of this struct)
    size_t nchunks_csum;
    size_t redo_size;
    size_t parity_row_size;
    size_t parity_num_rows;
    uint64_t csum_map_offset;
    uint64_t redo_offset;
    uint64_t parity_offset;
};

struct pavise_layout_header* pavise_layout_init(void* base, size_t chunk_size_csum, size_t nchunks_csum, size_t redo_size,
        size_t parity_row_size, size_t parity_num_rows);
void pavise_clwb(void* addr, size_t len);

#ifdef __cplusplus
}
#endif

#endif // PAVISE_ALLOC

