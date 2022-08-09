#ifndef PAVISE_INTERFACE_H
#define PAVISE_INTERFACE_H
#include "stdlib.h" 
#include "stdint.h"
#include "assert.h"
#ifdef __cplusplus
extern "C" {
#endif


int get_global();
void set_global(int val);
void __attribute__((noinline)) pavise_instrument_begin(void);
void __attribute__((noinline)) pavise_instrument_end(void);
void __attribute__((noinline)) pavise_recovery_begin(void);
void __attribute__((noinline)) pavise_recovery_end(void);
void* cpavise_mmap(void* addr, size_t length, int prot,
        int flags, int fd, off_t offset);
int cpavise_munmap(void* addr, size_t length);
uint32_t pavise_isal_adler32(uint32_t init, const unsigned char* buf, uint64_t len);
void cpavise_memcpy(void* dest, const void *src, size_t len, size_t inst_id);
void cpavise_memset(void* dest, int val, size_t len, size_t inst_id);
void cpavise_memmove(void* dest, const void *src, size_t len, size_t inst_id);
void cpavise_flush(const void *src, size_t len);
void cpavise_drain();
void cpavise_verify();
void cpavise_persist(const void *src, size_t len);
void cpavise_initialize(const char* path);
void cpavise_create(const char* path, size_t poolsize);
void cpavise_delete(const char* path);
void cpavise_open(const char* path);
void cpavise_close();
void* cpavise_mmap(void* addr, size_t len, int proto, int flags, int fd,
        off_t offset);
void cpavise_add_data(void* addr, size_t size);
int cpavise_invalidate_data(void* addr, size_t size);
int cpavise_check_data(void* addr, size_t size);
int cpavise_load_data(void* addr, size_t size, size_t inst_id);
int cpavise_store_data(void* addr, size_t size);
//__attribute__((always_inline)) extern 
int cpavise_store_data_fileline(void* addr, size_t size, int fileid, int line);
void cpavise_release_lock();
void cpavise_update_parity(void* addr, size_t size);
void cpavise_ntstore();

void* cpavise_allocbuf(void* ptr, size_t size, int copy);
void cpavise_pmemwrite(void* dst, size_t size);
void cpavise_tx_begin();
void cpavise_tx_end();
int cpavise_isclean(void* ptr);

#ifdef __cplusplus
}
#endif
#endif // PAVISE_INTERFACE
