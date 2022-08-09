#include "pavise_interface.h"
#include "pavise.hpp"
#ifdef __cplusplus
extern "C" {
#endif

static int my_global;

int get_global(){
    return my_global;
}

void set_global(int val){
    my_global = val;
    return;
} 

void __attribute__((noinline)) pavise_instrument_begin(void){std::cout << "Hello Pavise\n";return;}
void __attribute__((noinline)) pavise_instrument_end(void){std::cout << "Goodbye Pavise\n";return;}
void __attribute__((noinline)) pavise_recovery_begin(void){std::cout<<"Pavise: Begin Recovery\n";in_recovery = 1;return;}
void __attribute__((noinline)) pavise_recovery_end(void){std::cout<<"Pavise: End Recovery\n";in_recovery = 0; return;}
void* cpavise_mmap(void* addr, size_t length, int prot,
        int flags, int fd, off_t offset){
    void* ret = pavise_mmap(addr, length, prot, flags, fd, offset); 
    return ret;
}

int cpavise_munmap(void* addr, size_t length){
    return pavise_munmap(addr,length);
}

uint32_t pavise_isal_adler32(uint32_t init, const unsigned char* buf, uint64_t len){
    return isal_adler32(init, buf, len);
}

void cpavise_memcpy(void* dest, const void *src, size_t len, size_t inst_id){
    //printf("Memcpy: size = %lu\n", len);
    pavise_memcpy(dest,src,len, inst_id);
    return;
}

void cpavise_memset(void* dest, int val, size_t len, size_t inst_id){
    //printf("Memset: size = %lu\n", len);
    pavise_memset(dest,val,len);
    return;
}
void cpavise_memmove(void* dest, const void *src, size_t len, size_t inst_id){
    //printf("Memmove: size = %lu\n", len);
    pavise_memmove(dest,src,len, inst_id);
    return;
}
void cpavise_flush(const void *src, size_t len){
    assert(0);
    pavise_flush(src,len);
    return;
}
void cpavise_drain(){
    assert(0);
    pavise_drain();
    return;
}
void cpavise_verify(){
    assert(0);
    pavise_verify();
    //pavise_verify_periodic();
    return;
}
void cpavise_persist(const void *src, size_t len){
    pavise_persist(src,len);
    return;
}
void cpavise_initialize(const char* path){
    pavise_initialize(path);
}
void cpavise_create(const char* path, size_t poolsize){
    pavise_create(path, poolsize);
    //pavise_play_create(path, poolsize);
}
void cpavise_delete(const char* path){
    pavise_delete(path);
}
void cpavise_open(const char* path){
    pavise_open(path);
}
void cpavise_close(){
    pavise_close();
}

void cpavise_add_data(void* addr, size_t size) {
    assert(0);
    pavise_add_data(addr,size);
    return;
}
int cpavise_load_data(void* addr, size_t size, size_t inst_id) {
    // favise_load_data will add chunk to load_list
    pavise_load_data(addr,size,inst_id);
    return 0;
}
int cpavise_store_data(void* addr, size_t size) {
    //if(size >=65535)
        //std::cout << "direct store addr = " << addr << ", size = " << size << std::endl;
    pavise_store_data(addr,size);
    return 0;
}
//__attribute__((always_inline))
int cpavise_store_data_fileline(void* addr, size_t size, int fileid, int line){
    /*
    if(WITHIN_SHADOW(addr)){
        if(size == 0) return 0;
        redo_list.push_back(std::make_tuple((uintptr_t) addr, size));
    }
    return 0;
    */
    //if(size >=65535)
        //std::cout << "direct store_fileline addr = " << addr << ", size = " << size << std::endl;
    pavise_store_data_fileline(addr,size,fileid,line);
    return 0;

}

void cpavise_release_lock(){
    pavise_release_lock();
}

void* cpavise_allocbuf(void* ptr, size_t size, int copy){
    return NULL;
}

void cpavise_pmemwrite(void* dst, size_t size){
    return;
}

void cpavise_tx_begin(){
    //in_tx = 1;
    //pavise_mprotect(0x200003c0400,512,PROT_READ);
    //std::cout << "txbegin\n";
    //pavise_mprotect(0,0,PROT_NONE);
    //pavise_mprotect(0,0,PROT_READ | PROT_WRITE);
}
void cpavise_tx_end(){
    //in_tx = 0;
    //pavise_mprotect(0,0,PROT_READ | PROT_WRITE);
    //pavise_mprotect(0,0,PROT_READ);
    //std::cout << "txend\n";
    pavise_redo_process();
}

int cpavise_isclean(void* ptr){
    return pavise_isclean(ptr);
}
#ifdef __cplusplus
}
#endif
