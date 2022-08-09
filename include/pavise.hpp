#ifndef PAVISE_H
#define PAVISE_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <map>
#include <algorithm>
#include <cstdint>
#include <unistd.h>
#include <cstring>
#include <csignal>
#include <ctime>
#include <x86intrin.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cassert>
#include "igzip_lib.h"
#include "raid.h"
#include "crc.h"
#include "pavise_memops.h"
#include <thread>
#include <mutex>
#include <atomic>
#include <cerrno>

#ifdef DEBUG_ENABLED
#define DEBUG(x) x
#else
#define DEBUG(x) {}
#endif

#ifndef MAP_SYNC
#define MAP_SYNC 0x80000
#endif

#ifndef MAP_SHARED_VALIDATE
#define MAP_SHARED_VALIDATE 0x03
#endif

#define MMAP_UNMAPPED (void*)-1
#define MMAP_SUCCESS (void*) 0
#define PAVISE_POOL_HDR_LEN (0)
#define HASHMAP_SIZE (g_pavise_pool.len / sizeof(struct addr_chksum_map_entry))
#define PMEM_BASE ((uintptr_t)0x10000000000ull)
#define SHADOW_BASE ((uintptr_t)0x30000000000ull)
#define SP_DIFF (SHADOW_BASE - PMEM_BASE)
#define PAVISE_BASE ((uintptr_t)0x50000000000ull)
#define PART_BASE ((uintptr_t)0x60000000000ull)
#define PART_SHADOW_BASE ((uintptr_t)0x70000000000ull)
#define MAX_PATH_SIZE 256
//#define MAX_CHKSUM_RANGE 1024 
#define CSIZE 512ULL
#define CLSIZE 64ULL
#define COAL_THRESHOLD 128
#define REDO_COAL_THRESHOLD 128 
//#define CHUNKS_PER_PARITY 1
#define PARITY_NUM_ROWS 100

#define WITHIN_PMEM(a) ((uintptr_t)a >= PMEM_BASE && (uintptr_t)a < SHADOW_BASE)
#define WITHIN_SHADOW(a) ((uintptr_t)a >= SHADOW_BASE && (uintptr_t)a < PAVISE_BASE)
#define WITHIN_PART(a) ((uintptr_t)a >= PART_BASE && (uintptr_t)a < PART_SHADOW_BASE)
#define WITHIN_SHADOWPART(a) ((uintptr_t)a >= PART_SHADOW_BASE && (uintptr_t)a < PART_SHADOW_BASE + PMEM_BASE)
#define PMEM_OFFSET(a) ((uintptr_t)a - PMEM_BASE)
#define SHADOW_OFFSET(a) ((uintptr_t)a - SHADOW_BASE)

//global variables
extern int in_recovery;
extern int in_tx;
extern std::vector<struct mmap_entry> mmap_list;
extern std::unordered_multimap<uint16_t, uint64_t> mmap_map;
extern struct pavise_pool g_pavise_pool;

extern struct csum_entry* csum_map;
extern struct pavise_layout_header* g_layout_hdr;

// information of the PM pool created and managed by pavise
struct pavise_pool {
    int fd;
    std::string path;
    size_t len;
    void* addr;
    size_t parity_num_rows;
};

// struct containing information for each mmap mapping and corresponding file
struct mmap_entry {
    uint64_t addr_start;
    uint64_t addr_end;
    off_t foffset; 
    std::string fpath;
    uint16_t fpath_crc;
};


struct csum_entry {
    uintptr_t caddr;
    uint32_t chksum;
    uint32_t timestamp; 
};

struct redo_header {
    uint32_t chksum;
    uint32_t num_entries;
    uint64_t data_offset;
    uint64_t total_size;
};

struct redo_entry {
    uintptr_t uaddr : 48;
    uint16_t  size;
    uintptr_t saddr;
};

struct redo_memset_entry {
    int val;
    uint64_t size;
};

void __attribute__((constructor)) pavise_constructor();
void pavise_sig_handler(int sig, siginfo_t *si, void* unused);
void* pavise_mmap(void* addr, size_t length, int prot,
        int flags, int fd, off_t offset);
int pavise_munmap(void* addr, size_t length);
void pavise_initialize(const char* path);
void pavise_create(const char* path, size_t poolsize);
void pavise_play_create(const char* path, size_t poolsize);
void pavise_delete(const char* path);
void pavise_open(const char* path);
void pavise_close();
void pavise_memcpy(void* dest, const void *src, size_t len, size_t inst_id);
void pavise_memset(void* dest, int val, size_t len);
void pavise_memmove(void* dest, const void *src, size_t len, size_t inst_id);
void pavise_flush(const void *src, size_t len);
void pavise_drain();
void pavise_persist(const void *src, size_t len);
void pavise_add_data(void* addr, size_t size);
int pavise_load_data(void* addr, size_t size, size_t inst_id);
int pavise_store_data(void* addr, size_t size);
int pavise_store_data_fileline(void* addr, size_t size, int fileid, int line);
void pavise_release_lock();
int pavise_verify();
int pavise_verify_periodic();
int pavise_verify_all();
int pavise_verify_recover(void* check_addr,bool force);
int pavise_recover(void* check_addr);
int pavise_parity_column_memcmp(void* check_addr);
void pavise_flush_invalidated();
void pavise_invalidate(struct csum_entry* e);
void pavise_redo_process();
void pavise_redo_apply(struct redo_header* redo_hdr);
int pavise_isclean(void* ptr);
void pavise_mprotect(void* base, uint64_t len, int prot);

int pavise_check_data(void* addr, size_t size);
void pavise_update_parity(void* old_addr, void* new_addr, size_t size);
void pavise_persist_internal(void* addr, size_t len);
void pavise_flush_internal(void* addr, size_t len);
int hash_func(uint64_t addr, size_t hashmap_size);
inline uint64_t uaddr_map(void* uaddr);

void dump_memory_region(void* addr, size_t len);

// search through mmap_list and return the offset of the address from the file
inline
int64_t get_pm_offset(void* addr) {
    for(auto i = mmap_list.begin(); i != mmap_list.end(); i++){
        if((uintptr_t)addr >= (uintptr_t) i->addr_start &&
                (uintptr_t)addr < (uintptr_t) i->addr_end) {
            return (uintptr_t) addr - i->addr_start + i->foffset;
        }
    }
    return -1;
}

// from volatile address to unique address
inline
uint64_t uaddr_map(void* uaddr){
    //return (uint64_t) uaddr;
    uint64_t retaddr = (uint64_t) uaddr;
    for(auto i = mmap_list.begin(); i != mmap_list.end(); i++){
        if((uintptr_t)uaddr >= (uintptr_t) i->addr_start &&
                (uintptr_t)uaddr < (uintptr_t) i->addr_end) {
            uintptr_t mask = (uintptr_t) i->fpath_crc << 48;
            // get offset
            retaddr =  (retaddr - (uintptr_t) i->addr_start + i->foffset);
            // generate new address with most significant 16 bits being CRC16 of file path,
            // and LSB being the offset in the file
            retaddr =  ((retaddr & 0xffffffffffffull) | mask);
        
            //update cache
            //uaddr_map_cache = *i;

            return retaddr;
        }
    }
    return 0; 
}

// from unique address to volatile address
inline
uint64_t map_uaddr (uint64_t uaddr) {
    assert(0);
    uint16_t prefix = (uaddr & 0xffff000000000000ULL) >> 48;
    uint64_t offset =  uaddr & 0x0000ffffffffffffULL;
    auto it = mmap_map.find(prefix);
    if(it == mmap_map.end()){
        return 0;
    }
    //DEBUG(assert(it != mmap_map.end()));
    return it->second + offset;
}

inline
void get_only_base(uintptr_t& base, uintptr_t& end){
    if(mmap_list.size() != 1){
        end = 0;
        return;
    }
    base = mmap_list[0].addr_start;
    end = mmap_list[0].addr_end;
    return;
}
#endif // PAVISE
