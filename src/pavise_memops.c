#include "pavise_memops.h"

// Pass in base addr returned from mmap, inits a memory pool with fixed-size chunks
struct pavise_layout_header* pavise_layout_init(void* base, size_t chunk_size_csum, size_t nchunks_csum, size_t redo_size,
        size_t parity_row_size, size_t parity_num_rows){

    struct pavise_layout_header* hdr = (struct pavise_layout_header*) base;
    pavise_setvalid(hdr,false);

    hdr->nchunks_csum = nchunks_csum;
    //unsigned char* offset = (unsigned char*)base + PAVISE_LAYOUT_HDR_SIZE;
    uint64_t offset = PAVISE_LAYOUT_HDR_SIZE;

    // align address for redo log
    unsigned remainder = (uint64_t) offset % (sizeof(uint32_t)*2 + sizeof(uint64_t));
    if(remainder != 0) {
        offset =  ((uint64_t)offset + (sizeof(uintptr_t) + sizeof(size_t))- remainder);
    }
    // Reserve space for redo log
    hdr->redo_size = redo_size;
    hdr->redo_offset = offset;
    offset += redo_size;

    size_t csum_map_size = chunk_size_csum * nchunks_csum;

    // align address for csum hash map
    remainder = (uint64_t) offset % chunk_size_csum;
    if(remainder != 0) {
        offset = ((uint64_t)offset + chunk_size_csum - remainder);
    }
    // Reserve space for checksum entries
    hdr->csum_map_offset = offset;
    offset += csum_map_size;

    // align address for parity region
    remainder = (uint64_t) offset % PAGESIZE;
    if(remainder != 0) {
        offset = ((uint64_t)offset + PAGESIZE - remainder);
    }
    // Reserve space for parity region
    hdr->parity_offset = offset;
    hdr->parity_num_rows = parity_num_rows;
    hdr->parity_row_size = parity_row_size;
    offset += parity_row_size;
                
    hdr->hdr_size = (uint64_t) offset - (uint64_t)base;

    pavise_setvalid(hdr, true);

    return hdr;
}


void pavise_clwb(void* addr, size_t len){
    // Same as flush_clwb_nolog in PMDK, loop through cache lines in the given range
    for( uintptr_t ptr = (uintptr_t) addr & ~(PCACHELINE_SIZE - 1);
            ptr < (uintptr_t) addr + len; ptr += PCACHELINE_SIZE) {
        _mm_clwb((void*)ptr);
    }
}
