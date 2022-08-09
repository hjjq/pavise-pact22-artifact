#include "pavise.hpp"
#define XOR_GEN_MASK (~(32ULL - 1))
#define CSIZE_MASK (~(CSIZE - 1))
#define CLSIZE_MASK (~(CLSIZE - 1))
#define PAGE_MASK (~(PAGESIZE - 1))
#define GET_CSUM(key)  ((struct csum_entry*)(&(csum_map[hash_func(key, g_layout_hdr->nchunks_csum)])))
#define GET_CSUM_CONST(key)  ((const struct csum_entry*)(&(csum_map[hash_func(key, g_layout_hdr->nchunks_csum)])))
#define GET_CSUM_INDEX(i)  ((struct csum_entry*)(&(csum_map[i])))
#define ABSDIFF(a,b) ( (a > b) ? (a - b) : (b - a))
#define WITHIN_RANGE(a) ((uintptr_t)a >= 0x100003fe400ULL && (uintptr_t)a < 0x100003fe600ULL)
#define WITHIN_RANGE_REAL(a) ((uintptr_t)a >= 0x10003effe00ULL && (uintptr_t)a <= 0x10003f00000ULL)
#define passert(a) assert(a)

// redirection related data structures
/*
thread_local std::vector<std::tuple<uintptr_t, uintptr_t, size_t>> redo_list; //<shadow,pm,size>
thread_local std::unordered_set<uintptr_t> unique_addrs;
thread_local std::vector<std::tuple<uintptr_t, uintptr_t, size_t, int>> memset_list; //<shadow,pm,size,val>
thread_local std::unordered_map<uintptr_t, std::tuple<uintptr_t, uintptr_t, size_t,size_t>> load_list; //<shadow,pm,size,inst_id>
std::unordered_set<unsigned char*> already_computed; 
*/
// memcached-lenovo requires redo_list to be thread_local to work properly
//thread_local std::vector<std::tuple<uintptr_t, uint32_t>> redo_list; //<shadow,pm,size>
std::vector<std::tuple<uintptr_t, uint32_t>> redo_list; //<shadow(offset),size>
//thread_local std::vector<std::tuple<uintptr_t, uint32_t>> redo_list; //<shadow,size>
std::unordered_set<uintptr_t> unique_addrs;
std::vector<std::tuple<uintptr_t, uint32_t, int>> memset_list; //<shadow,size,val>
//std::unordered_map<uintptr_t, std::tuple<uintptr_t, uintptr_t, size_t, size_t>> load_list; //<shadow,pm,size,inst_id>
//thread_local std::vector<std::tuple<uintptr_t, uintptr_t, size_t>> load_list; //<shadow,pm,size,inst_id>
std::vector<std::tuple<uintptr_t, uintptr_t, size_t>> load_list; //<shadow,pm,size,inst_id>
std::unordered_set<unsigned char*> already_computed; 
std::unordered_set<char*> pages_allocated; 
std::unordered_set<char*> csum_computed;

//std::unordered_set<void *> mismatch_addr; 

std::atomic<bool> final_process(true);

thread_local unsigned ccount = 0;
unsigned tx_count = 0;
bool first_passed = 0;

std::mutex plock;
thread_local bool holds_lock = false;

bool blacklist_lines[256];

struct redo_record {
    uintptr_t shadow;
    uintptr_t pm;
    size_t size;
};

//struct redo_record redo_array[30];
//int redo_array_size = 0;
bool main_pool_mapped = 0;
bool main_pool_big_mapped = 0;
bool part_mapped = 0;
int in_recovery;
int in_tx = 0;
int memcpycnt = 0;
double memcpytime = 0.0;
double paritytime = 0.0;
double preptime = 0.0;
double veritime = 0.0;
double logtime = 0.0;
double discardtime = 0.0;
double headertime = 0.0;
uint64_t memcpysize = 0;

int rsuccess = 0;
int rfail = 0;
int printcount = 0;

struct pavise_pool g_pavise_pool = {
    -1, //fd
    "", //path
    0, //len
    MMAP_UNMAPPED, //addr
    0 // parity_num_rows
};

// List of memory mapped regions
std::vector<struct mmap_entry> mmap_list;
// Map file path crc to virtual address of mapping
std::unordered_multimap<uint16_t, uint64_t> mmap_map;
// List of fd's already mapped
std::unordered_set<int> mapped_fds;


struct csum_entry* csum_map;
size_t nchunks_csum;
struct pavise_layout_header* g_layout_hdr;
uintptr_t prev_addr = 0;

void __attribute__((constructor)) pavise_constructor(){
    // Setup signal handler
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = pavise_sig_handler;
    if(sigaction(SIGSEGV, &sa, NULL) == -1) {
        std::cout << "sigaction failed\n";
        assert(0);
    }
}

int memcmp_pos(char* p1, char* p2, size_t count){
    int v = 0;
    int pos = 0;
    int first_pos = 0;
    while(count-- > 0){ //&& v ==0) {
        v = *(p1++) - *(p2++);
        if(v == 0){
            //std::cout << (void*)p1 << " matches " << (void*) p2 << std::endl;
        }
        else{
            //std::cout << "memcmp mismatch at " << (void*)(p1 - 1) << ", pos = " << pos << std::endl;
            first_pos = first_pos == 0 ? pos : first_pos;
            //break;
        }
        pos++;
    }
    return first_pos;
}

void pavise_sig_handler(int sig, siginfo_t *si, void* unused){
    //plock.lock();
    void* saddr = (uintptr_t)si->si_addr & PAGE_MASK;
    void* paddr = (uintptr_t)saddr - SP_DIFF;
    mprotect(saddr, PAGESIZE, PROT_READ | PROT_WRITE);
    memcpy(saddr, paddr, PAGESIZE);
    //pages_allocated.insert((char*) saddr);
    //std::cout << "Got SIGSEGV at address: " << saddr << std::endl;
    //passert(0);
    //plock.unlock();
}

void pavise_helper_thread(struct redo_header* redo_hdr, std::unordered_map<uint64_t, unsigned char*>& chunks,
        std::unordered_map<uint64_t, unsigned char*>& zero_chunks){

    redo_hdr->chksum = isal_adler32(0, (unsigned char*) (redo_hdr + 1), redo_hdr->total_size - sizeof(struct redo_header));
    pavise_persist_internal(redo_hdr,  redo_hdr->total_size);
    pavise_redo_apply(redo_hdr);
    for(auto it = chunks.begin(); it != chunks.end(); it++) {
        unsigned char* check_addr = it->second;
        pavise_flush_internal(check_addr, CSIZE);
    }
    for(auto it = zero_chunks.begin(); it != zero_chunks.end(); it++) {
        unsigned char* check_addr = it->second;
        pavise_flush_internal(check_addr, CSIZE);
    }
    PSFENCE();

    // discard redo log
    memset(redo_hdr, 0, sizeof(struct redo_header));
    pavise_persist_internal(redo_hdr, sizeof(struct redo_header));
    chunks.clear();
    zero_chunks.clear();
    plock.unlock();
    return;
}
char temphdr [4096];
uintptr_t tempaddr;
void* pavise_mmap(void* addr, size_t length, int prot,
        int flags, int fd, off_t offset) {

    std::cout << "pavise_mmap called: addr = " << addr << ", fd = " << fd << ", length = " << length
        << ", offset = " << offset<< std::endl;
    //return mmap(addr,length,prot,flags,fd,offset);
    /*
    void* reta = mmap(addr,length,prot,flags,fd,offset);
    std::cout << "returning " << reta << std::endl;
    return reta;
    */

    if(fd == -1) { // nvm_malloc calls an anonymous mapping
        addr = (void*)SHADOW_BASE;
        std::cout << "fd = -1, len = " << length << ", returning " << addr << std::endl;
        //return mmap(addr,length,prot,flags,fd,offset);
        return addr; 
    }
    // HACK: the previous call with fd = -1 will return SHADOW_BASE, which will then
    // get passed into this call. But what we really want addr to be is PMEM_BASE
    if((uintptr_t) addr == SHADOW_BASE){ 
        std::cout << "converting\n";
        addr = (void*) PMEM_BASE;
    }

    // HACK: PMDK maps parts, create PART_BASE and PART_SHADOW_BASE mappings
    if((uintptr_t) addr == 0) {
        part_mapped = true;
        if(main_pool_mapped){
            std::cout << "returning SHADOW_BASE for part\n";
#ifdef PAVISE_PINTOOL
            return PMEM_BASE;
#else
            return SHADOW_BASE;
#endif
        }
        //addr = (void*) PART_BASE;
    }
    else{
        main_pool_mapped = true;
    }
    // Find the path of the file we're trying to mmap, record it in mmap_list
    char path[MAX_PATH_SIZE];
    std::string fdpath = "/proc/self/fd/" + std::to_string(fd);
    ssize_t ret = readlink( fdpath.c_str(), path, MAX_PATH_SIZE);
    if(ret == -1 || ret == MAX_PATH_SIZE){
        std::cout<< "pavise_mmap: readlink failed. ret = " << ret << std::endl;
        return MAP_FAILED;
    }
    path[ret] = '\0';
    std::string fpath = path;

    void* mmap_addr = mmap(addr, length, prot, flags, fd, offset);

    if(mmap_addr != MAP_FAILED) {
        uint64_t end = ((uintptr_t)mmap_addr + length);
        uint16_t crc = crc16_t10dif(0, (const unsigned char*)fpath.c_str(), fpath.size());
        struct mmap_entry mmap_e = {
            .addr_start = (uint64_t)mmap_addr,
            .addr_end = end,
            .foffset = offset,
            .fpath = fpath,
            .fpath_crc = crc
        };
        mmap_list.push_back(mmap_e);
        //mmap_map.insert(std::make_pair(crc, (uint64_t)mmap_addr));
        DEBUG(std::cout << "pavise_mmap recorded start = " << mmap_addr << ", length = " << length
                << ", file = " << fpath << ", offset = " << offset << std::endl;)
    }
    void* shadow_addr = mmap_addr;
    unsigned char c;
    /*
    for(int idx = 0; idx < length; ++idx){
        c = *((unsigned char*)shadow_addr + idx);
        if(c != 0) {
            std::cout << "non zero at byte" << idx << std::endl;
            assert(0);
        }
    }
    */
#ifndef PAVISE_PINTOOL
    if(WITHIN_PMEM(addr)){
        std::cout << "input to mmap is  " <<(void*)(SHADOW_BASE + PMEM_OFFSET(addr))<< std::endl;
        shadow_addr = mmap((void*)(SHADOW_BASE + PMEM_OFFSET(addr)), length, prot,MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        //shadow_addr = mmap((void*)(SHADOW_BASE + PMEM_OFFSET(addr)), length, prot, MAP_PRIVATE, fd, offset);
        //shadow_addr = mmap((void*)SHADOW_BASE, length, prot, flags, fd, offset);
        if(shadow_addr == MAP_FAILED){
            perror("mmap failed");
            passert(0);
        }
        pavise_mprotect(shadow_addr,length, PROT_NONE); 
        main_pool_mapped = true;
    }
    else if(WITHIN_PART(addr)) { // deal with partial mappings (0x7ff...) in PMDK
        //std::cout << "else " << mmap_addr << std::endl;
        if(main_pool_mapped){
            shadow_addr = SHADOW_BASE;
        }
        else{
            //shadow_addr = mmap((void*)(PART_SHADOW_BASE + offset), length, prot, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
            shadow_addr = mmap((void*)(PART_SHADOW_BASE + offset), length, prot, MAP_PRIVATE, fd, offset);
        }
        if(shadow_addr == MAP_FAILED) {
            perror("shadowpart mmap failed");
            passert(0);
        }
        part_mapped = true;
    }
#endif
    std::cout << "returning " << shadow_addr << std::endl;
    if((uintptr_t)addr == 0){
        memcpy(temphdr, shadow_addr, 4096);
        tempaddr =(uintptr_t) shadow_addr;
    }
    return shadow_addr; 
}

int pavise_munmap(void* addr, size_t length){
    if( (uint64_t)addr == PMEM_BASE){
        pavise_flush_invalidated();
        pavise_drain();
    }
    std::cout << "pavise_munmap called: addr = " << addr << ", length = " << length << std::endl;
    if(part_mapped){
        //std::cout << "pavise_munmap: part_mapped\n";
        part_mapped = false;
        pavise_redo_process();
        if (main_pool_mapped){
            //int* temp_ptr = (int*)((uintptr_t)0x30000000000);
            //*temp_ptr = 0;
            //std::cout << "pavise_munmap: part_mapped, returning\n";
            return 0;
        }
    }
    if(WITHIN_SHADOW(addr)){
        //std::cout << "pavise_munmap: within_shadow\n";
        if(part_mapped) {
            part_mapped = false;
            pavise_redo_process();
            return 0;
            addr = (void*)PART_BASE;
            std::cout << "unmapping part\n";
            int ret = munmap(PART_BASE,length);
            //successful munmap, remove from mmap_list and mmap_map
            if(!ret) {
                std::cout << "ummap successful\n";
                for(auto it = mmap_list.begin(); it != mmap_list.end(); it++){
                    if(it->addr_start == (uint64_t)addr) {
                        mmap_list.erase(it);
                        break;
                    }
                }
                for(auto it = mmap_map.begin(); it != mmap_map.end(); it++) {
                    if(it->second == (uint64_t)addr) {
                        mmap_map.erase(it);
                        break;
                    }
                }
            }
            return;
        }
        else{
            std::cout << "pavise_munmap calling pavise_redo_process, size of redo_list = " << redo_list.size() << "\n";
            pavise_redo_process();
            //std::cout << "memcpy cnt = " << memcpycnt << ", total size = " << memcpysize << ", total time = " << memcpytime<< std::endl;
            //int pos = memcmp_pos(PMEM_BASE, SHADOW_BASE, length);
            //std::cout << "final memcmp, pos = " << pos << std::endl;
            std::cout << "redoapply = " << memcpytime << ", parity = " << paritytime << ", prep = " << preptime<< ", discard = " << discardtime<<
                ", header = " << headertime << std::endl;
            std::cout << "#s=" << rsuccess << "#f=" << rfail << "#end" << std::endl;
            std::cout << "pages allocated = " << pages_allocated.size() << ", checksums computed = " << csum_computed.size()<<std::endl;
            main_pool_mapped = false;
            //dump_memory_region((void*)0x100003fe400ULL, 512);
            //pavise_parity_column_memcmp((void*)0x100003fe400ULL);
            //dump_memory_region((void*)0x100003fe400ULL, 512);
        }
    }
    std::cout << "unmapping " << addr << std::endl;
    /*
    //if((uintptr_t)addr == PART_SHADOW_BASE){
    if((uintptr_t)addr == tempaddr){
        int res = memcmp_pos(addr, temphdr,PAGESIZE);
        std::cout << "memcmp res = " << res << std::endl;
    }
    */
    int ret = munmap(addr,length);
    //successful munmap, remove from mmap_list and mmap_map
    if(!ret) {
        for(auto it = mmap_list.begin(); it != mmap_list.end(); it++){
            if(it->addr_start == (uint64_t)addr) {
                mmap_list.erase(it);
                break;
            }
        }
        for(auto it = mmap_map.begin(); it != mmap_map.end(); it++) {
            if(it->second == (uint64_t)addr) {
                mmap_map.erase(it);
                break;
            }
        }
    }

    return 0;
}

void pavise_play_create(const char* path, size_t poolsize){

    std::cout << "\n\n===================== creating toy pool ====================\n\n";

    int flags = O_RDWR | O_CREAT | O_EXCL;
    std::string pavise_path = path;
    size_t len = poolsize;


    while (pavise_path.back() == '/') pavise_path.pop_back();
    pavise_path += "_toy_pool";

    int fd = open(pavise_path.c_str(), flags, 0666);

    if(fd < 0) {
        std::cout << "pavise_play_create: failed to open file, path = " << pavise_path << std::endl;
        return;
    }

    if(posix_fallocate(fd, 0, len) != 0) {
        std::cout << "pavise_toy_create: failed to fallocate space for new file, len = " << len << std::endl;
        return;
    }

    // mmap the opened file
    int prot = PROT_READ | PROT_WRITE;
    flags = MAP_SHARED_VALIDATE | MAP_SYNC; 
    //flags = MAP_SHARED | MAP_SYNC; 
    void *ret = mmap((void*)PART_BASE, len, prot, flags, fd, 0);
    ret = mmap((void*)PART_SHADOW_BASE, len, prot, MAP_PRIVATE, fd, 0);

    if (ret != MAP_FAILED) {
        memset(ret, 0, len);
        //pavise_persist_internal(ret, len);
        DEBUG(std::cout << "pavise_play_create: mmap successed, addr ="<< ret << ", len = "<< len << std::endl;);

    } else {
        std::cout << "pavise_play_create: mmap failed" << std::endl;
        passert(0);
    }
    
    return;
}
void pavise_initialize(const char* path){
    uint64_t guess_size = 1024*1024*1024*6ULL; // 6GB
    pavise_create(path, guess_size);
}
void pavise_create(const char* path, size_t poolsize){
    DEBUG(std::cout << "pavise_create: path = " << path << ", poolsize = " << poolsize << std::endl;);
    std::cout << "\n\n===================== Enhanced by Pavise ====================\n\n";
    std::cout << "\n\n!!! Running pavise pact 22 test \n\n";
#ifdef PAVISE_PINTOOL
    return;
#endif
    passert(g_pavise_pool.fd == -1);
    int flags = O_RDWR | O_CREAT | O_EXCL;
    std::string pavise_path = path;
    //PACT22, quick hack for low N ratio for rtree
    //size_t len = poolsize*3;
    size_t len = poolsize;


    while (pavise_path.back() == '/') pavise_path.pop_back();
    pavise_path += "_pavise_pool";

    int fd = open(pavise_path.c_str(), flags, 0666);

    if(fd < 0) {
        std::cout << "pavise_create: failed to open file, path = " << pavise_path << std::endl;
        return;
    }

    if(posix_fallocate(fd, 0, len) != 0) {
        std::cout << "pavise_create: failed to fallocate space for new file, len = " << len << std::endl;
        return;
    }

    // mmap the opened file
    int prot = PROT_READ | PROT_WRITE;
    flags = MAP_SHARED_VALIDATE | MAP_SYNC; 
    //flags = MAP_SHARED | MAP_SYNC; 
    void *ret = mmap((void*)PAVISE_BASE, len, prot, flags, fd, 0);

    if (ret != MAP_FAILED) {
        memset(ret, 0, len);
        pavise_persist_internal(ret, len);
        DEBUG(std::cout << "pavise_create: mmap successed, addr ="<< ret << ", len = "<< len << std::endl;);
        g_pavise_pool.fd = fd;
        g_pavise_pool.path = pavise_path; 
        g_pavise_pool.len = len;
        g_pavise_pool.addr = ret;

        size_t chunk_size_csum = sizeof(struct csum_entry);

        size_t used_size = PAVISE_LAYOUT_HDR_SIZE;

        nchunks_csum = (len - used_size) / 3 / chunk_size_csum;
        size_t csum_map_size = chunk_size_csum * nchunks_csum;
        used_size += csum_map_size;

        size_t redo_size = (len - used_size) / 3;
        used_size += redo_size;

        size_t parity_num_rows = PARITY_NUM_ROWS; // Parity will use 1/100 of user pool space
        size_t parity_row_size = len / parity_num_rows;

        // Adjust the parameters so that they all align to 32B
        parity_row_size = (parity_row_size) & XOR_GEN_MASK;
        parity_num_rows = len / parity_row_size;
        g_pavise_pool.parity_num_rows = parity_num_rows;

        std::cout << "len = " << len << ", parity_row_size = " << parity_row_size << ", parity_num_rows = " << parity_num_rows<< std::endl;
        struct pavise_layout_header* hdr = pavise_layout_init(ret, chunk_size_csum, nchunks_csum, redo_size,
                parity_row_size, parity_num_rows);
        csum_map = (struct csum_entry*)((uintptr_t)ret + hdr->csum_map_offset);
        g_layout_hdr = hdr;

    } else {
        std::cout << "pavise_create: mmap failed" << std::endl;
        passert(0);
    } 
    return;
}
void pavise_delete(const char* path){
    return;
}
void pavise_open(const char* path){
    std::cout << "pavise_open: path = " << path << std::endl;
    passert(g_pavise_pool.fd == -1);

    int flags = O_RDWR;
    std::string pavise_path = path;

    // Trim any trailing slashes in the path and append suffix
    while (pavise_path.back() == '/') pavise_path.pop_back();
    pavise_path += "_pavise_pool";

    int fd = open(pavise_path.c_str(), flags, 0666);

    if(fd < 0) {
        std::cout << "pavise_open: failed to open file, path = " << pavise_path << std::endl;
        return;
    }

    // Get the file size
    struct stat st;
    if( fstat(fd, &st) != 0) {
        std::cout << "pavise_open: failed to get file size" << std::endl;
        return;
    }
    //PACT22, quick hack for low N ratio for rtree
    //size_t len = st.st_size*3;
    size_t len = st.st_size;

    // mmap the opened file
    int prot = PROT_READ | PROT_WRITE;
    flags = MAP_SHARED_VALIDATE | MAP_SYNC;

    void *ret = mmap((void*)PAVISE_BASE, len, prot,
            flags, fd, 0);

    if (ret != MAP_FAILED) {
        DEBUG(std::cout << "pavise_open: mmap successed, addr ="<< ret << ", len = "<< len << std::endl;);
        g_pavise_pool.fd = fd;
        g_pavise_pool.path = pavise_path; 
        g_pavise_pool.len = len;
        g_pavise_pool.addr = ret;

        struct pavise_layout_header* hdr = (struct pavise_layout_header*) ret;

        csum_map = (struct csum_entry*)((uintptr_t)ret + hdr->csum_map_offset);
        g_layout_hdr = hdr;
        nchunks_csum = hdr->nchunks_csum;
        g_pavise_pool.parity_num_rows = hdr->parity_num_rows;
        //std::cout << "testing segfault\n";
        //int* testptr = (int*)((uintptr_t)0x50003ac5200);
        //*testptr=0;
        //std::cout << "testing segfault end\n";

    } else {
        std::cout << "pavise_open: mmap failed" << std::endl;
        passert(0);
    }

    return;
}
void pavise_close(){
    // actually, no need to do any unmaps, since mmap stil keeps reference to fd even after a close
    return;

    DEBUG(std::cout << "pavise_close" << std::endl;)

    if(g_pavise_pool.fd == -1 && g_pavise_pool.addr == MMAP_UNMAPPED){
        return;
    }

    if(munmap(g_pavise_pool.addr, g_pavise_pool.len) != 0) {
        std::cout << "pavise close: failed to unmap addr: " << g_pavise_pool.addr << ", len: " << g_pavise_pool.len << std::endl;
    }
    g_pavise_pool.addr = MMAP_UNMAPPED;
    if (close(g_pavise_pool.fd) != 0){
        std::cout << "pavise_close: failed to close fd: " << g_pavise_pool.fd << std::endl;
    }
    g_pavise_pool.fd = -1;

    return;
}

void pavise_memcpy(void* dest, const void *src, size_t len, size_t inst_id){
    //std::cout << "pavise memcpy addr = " << dest << ", size = " << len << std::endl;
#ifdef PAVISE_COLLECT_LOAD
    pavise_load_data(src,len,inst_id );
#endif
    pavise_store_data(dest, len);
    return;
}
void pavise_memset(void* dest, int val, size_t len){
    //if(!in_tx) return 0;
#ifdef PAVISE_PINTOOL
    if(WITHIN_PMEM(dest)){
        std::cout << "pavise memset: addr = " << dest << ", len = " << len << ", end = " << (void*)((uintptr_t)dest + len) << std::endl; //", val = " << val<< std::endl;
    }
    return;
#endif
    if(WITHIN_SHADOW(dest)){
        /*
        passert(holds_lock == false);
        plock.lock();
        holds_lock = true;
        */
        //if(WITHIN_RANGE(dest))
            //std::cout << "pavise memset: addr = " << dest << ", len = " << len << ", val = " << val<< std::endl;
        //std::cout << "pavise memset: addr = " << dest << ", len = " << len << ", end = " << (void*)((uintptr_t)dest + len) << ", val = " << val<< std::endl;
        memset_list.push_back(std::make_tuple((uintptr_t) dest, len, val));

        //pavise_redo_process();
    }
    else{
        return 0;
        if(get_pm_offset(dest) != -1){
            //std::cout << "pavise_memset: encounter addr " << dest << ", offset = " << get_pm_offset(dest)<<std::endl;
            //memset_list.push_back(std::make_tuple((uintptr_t)SHADOW_BASE + get_pm_offset(dest) , (uintptr_t) PMEM_BASE + get_pm_offset(dest), len,val));
        }
    }
    return;
}
void pavise_memmove(void* dest, const void *src, size_t len, size_t inst_id){
    //std::cout << "memmove addr = " << dest << ", size = " << len << std::endl;
#ifdef PAVISE_COLLECT_LOAD
    pavise_load_data(src,len,inst_id);
#endif
    pavise_store_data(dest, len);
    return;
}
void pavise_flush(const void *src, size_t len){
    return;
    if(!in_tx) return;
    if((uintptr_t)src >= SHADOW_BASE && (uintptr_t)src < PAVISE_BASE ){
        if(len == 0) return;
        redo_list.push_back(std::make_tuple((uintptr_t) src, len));
        //printf("store data: addr = %p, size = %lu\n", addr, size);
        if(len >= 65535) {
            std::cout << "size takes larger than 65535\n";
            passert(0);
        }
        
        //redo_array[redo_array_size] = {(uintptr_t) addr, (uintptr_t) addr - SP_DIFF, size};
        //redo_array_size++;
        //passert(redo_array_size <30);
    }
}
void pavise_persist(const void *src, size_t len){
    return;
}

void pavise_add_data(void* addr, size_t size){
    return;
}

int pavise_load_data(void* addr, size_t size, size_t inst_id){
#ifdef PAVISE_PINTOOL
    return 0;
#endif
#ifndef PAVISE_COLLECT_LOAD
    passert(0);
#endif
    //return 0;
    if((uintptr_t)addr >= SHADOW_BASE && (uintptr_t)addr < PAVISE_BASE ){
        // as vector of tuple
        load_list.push_back(std::make_tuple((uintptr_t) addr, (uintptr_t) addr - SP_DIFF, size));
        // as unordered_map of (shadow,tuple)
        //load_list.insert({(uintptr_t)addr,std::make_tuple((uintptr_t) addr, (uintptr_t) addr - SP_DIFF, size, inst_id)});
        //std::cout << "load at " << addr << ", inserting " << (void*)((uintptr_t) addr - SP_DIFF) << ", size = " << size << std::endl;
    }
    return 0;
}
int pavise_store_data_fileline(void* addr, size_t size, int fileid, int line){
    //if(WITHIN_RANGE(addr))
    /*
    if((uintptr_t)addr >= SHADOW_BASE && (uintptr_t)addr < PAVISE_BASE ){
        if(WITHIN_RANGE(addr))
            std::cout << "line num = " << std::dec <<  line << ", ";
    }
    */
    if(line == 193 && size <= 1) {
        return 0;
    }
    //if(size <= 1)
    //if(WITHIN_SHADOW(addr))
        //std::cout << "pavise store: addr = " << addr << ", len = "<< std::dec  << size  << ", line = " << line << std::hex << std::endl;
        //return 0;
    return pavise_store_data(addr,size);
    if(fileid == 1){
        if(!blacklist_lines[line]){
            //std::cout << "storing to blacklist: " << line << ", flag = " << blacklist_lines[line] << std::endl;
            return pavise_store_data(addr,size);
        }
        else{
            //std::cout << "skipping log for line " << line << ", addr = " << addr << ", size = " << size << std::endl;
        }
    }
    else
        return pavise_store_data(addr,size);
    return 0;
}
int pavise_store_data(void* addr, size_t size){
#ifdef PAVISE_PINTOOL
    if(WITHIN_PMEM(addr)){
        if(size == 0) return 0;
        std::cout << "pavise store: addr = " << addr << ", len = " << size << ", end = " << (void*)((uintptr_t)addr+size)<< std::endl;
    }
    return 0;
#endif
    //if(!in_tx) return 0;
    //if((uintptr_t)addr == 0x20003effe6bULL)
    //if(WITHIN_RANGE(addr))
            //std::cout << "pavise store: addr = " << addr << ", len = " << size << std::endl;
    if(WITHIN_SHADOW(addr)){
        if(size == 0) return 0;
        //if((uintptr_t)addr == 0x20003effe6bULL)
        //if(WITHIN_RANGE(addr))
        //if((uintptr_t)addr == 0x30000000fc8ULL)
            //std::cout << "pavise store: addr = " << addr << ", len = " << size << std::endl;
        /*
        passert(holds_lock == false);
        plock.lock();
        holds_lock = true;
        */
        //std::cout << "pavise store: addr = " << addr << ", len = " << size << ", end = " << (void*)((uintptr_t)addr+size)<< std::endl;
        plock.lock();
        redo_list.push_back(std::make_tuple( SHADOW_OFFSET(addr) , size));
            
        plock.unlock();

        //pavise_redo_process();

        if(size >= 65535) {
            std::cout << "size takes larger than 65535\n";
            passert(0);
        }
        //uintptr_t dummyshadow = (uintptr_t) addr + 0x30000000000ULL;// write to 0x500...., the shadow of play_pool
        //uintptr_t dummypavise = (uintptr_t) addr + 0x10000000000ULL;// 0x300.....
        //memset(dummyshadow, 6, size); 
        //memcpy(dummypavise, dummyshadow, size); // read from dummy, write to pavise (where we write does not actually matter)
        //memcpy(dummyshadow, dummypavise, size); // write to dummy random data
        //redo_array[redo_array_size] = {(uintptr_t) addr, (uintptr_t) addr - SP_DIFF, size};
        //redo_array_size++;
        //passert(redo_array_size <30);
    }
    else if (WITHIN_SHADOWPART(addr)){
        //return 0;
        
        std::cout << "pavise_store_data: encounter addr " << addr <<std::endl;
        redo_list.push_back(std::make_tuple((uintptr_t)addr - PART_SHADOW_BASE ,  size));

        //pavise_redo_process();

        /*
        if(get_pm_offset(addr) != -1){
            //std::cout << "pavise_store_data: encounter addr " << addr << ", offset = " << get_pm_offset(addr)<<std::endl;
            redo_list.push_back(std::make_tuple((uintptr_t)SHADOW_BASE + get_pm_offset(addr) ,  size));
        }
        */
    }
    return 0;
    uintptr_t page = (uintptr_t)addr & PAGE_MASK;
    uintptr_t page_end = ((uintptr_t)addr + size - 1) & PAGE_MASK;
    if(page >= PMEM_BASE*2 && page < PMEM_BASE*2 + PMEM_BASE){
        while (page <= page_end){
            //if(dirty_pages.find(page) == dirty_pages.end()){
                //std::cout << "store: inserting " << (void*)page << std::endl;
                //if(page == 0x100005c5000ULL){
                //    std::cout << "store: 5c5000\n";
                //    store_enct = 1;
                //}
            //}
            //dirty_pages.insert(page);
            page += PAGESIZE;
        }
    }
    return 0;
}

int pavise_verify(){
#ifdef PAVISE_NV
    return 0;
#endif
    std::unordered_set<unsigned char*> chunks;
#ifdef PAVISE_VERIFY_LOAD
    for(auto it = load_list.begin();it != load_list.end(); it++){
        //std::cout << "!!! size of laod list " << load_list.size() << std::endl;
#ifndef PAVISE_COLLECT_LOAD
        passert(0);
#endif
        // for vector, just use (*it) instead of (it->second)
        void* shadow = (void*)std::get<0>(*it);
        void* real = (void*)std::get<1>(*it);
        size_t size = std::get<2>(*it);
        /*
        void* shadow = (void*)std::get<0>(it->second);
        void* real = (void*)std::get<1>(it->second);
        size_t size = std::get<2>(it->second);
        size_t inst_id = std::get<3>(it->second);
        */
        //std::cout << "load at " << shadow <<", real = " << real << ", size = " << size << std::endl;

        uintptr_t uaddr_start = uaddr_map((void*)real);
        uintptr_t uaddr_end = uaddr_start + size - 1;
        uint64_t chunk_start = uaddr_start & CSIZE_MASK;
        uint64_t chunk_end = uaddr_end & CSIZE_MASK;

        // Loop through covered chunks
        int nchunks = ( chunk_end - chunk_start ) /  CSIZE + 1;
        for(int j = 0; j < nchunks; j++){
            uint64_t key = chunk_start + j*CSIZE;
            unsigned char* check_addr = (unsigned char*) (((uintptr_t)real & CSIZE_MASK) + j*CSIZE);
            chunks.insert(check_addr);
        }
    }
#endif
#ifdef PAVISE_VERIFY_STORE
    uintptr_t base,sp_diff;
    if(main_pool_mapped){
        base = SHADOW_BASE;
        sp_diff = SP_DIFF;
    }
    else{
        base = PART_SHADOW_BASE;
        sp_diff = PMEM_BASE;
    }
    for(auto it = redo_list.begin(); it != redo_list.end(); it++) {
        uintptr_t shadow = std::get<0>(*it) + base;
        uintptr_t real = shadow - sp_diff;
        uint32_t size = std::get<1>(*it);

        uintptr_t uaddr_start = uaddr_map((void*)real);
        uintptr_t uaddr_end = uaddr_start + size - 1;
        uint64_t chunk_start = uaddr_start & CSIZE_MASK;
        uint64_t chunk_end = uaddr_end & CSIZE_MASK;

        // Loop through covered chunks
        int nchunks = ( chunk_end - chunk_start ) /  CSIZE + 1;
        for(int j = 0; j < nchunks; j++){
            uint64_t key = chunk_start + j*CSIZE;
            unsigned char* check_addr = (unsigned char*) (((uintptr_t)real & CSIZE_MASK) + j*CSIZE);
            chunks.insert(check_addr);
        }
    }
#endif
    for(auto it = chunks.begin(); it != chunks.end(); it++){
        pavise_verify_recover(*it,false);
    }
    return 0;
}

void pavise_drain(){
    return;
}

int pavise_verify_periodic(){
    return 0;
}

int pavise_verify_all(){
    std::cout << "beginning pool scan\n";
    // Verification
    for(unsigned index = 0; index < g_layout_hdr->nchunks_csum; index++){
        struct csum_entry* e =  GET_CSUM_INDEX(index);
        if(e->timestamp == 0xdeadbeef){
            //std::cout << (void*) e->caddr << std::endl;
            pavise_verify_recover(e->caddr - SP_DIFF, true);
        }
    }
    first_passed = true;
    std::cout << "ending pool scan\n";

    return 0;
}

int pavise_parity_column_memcmp(void* check_addr){
    passert(WITHIN_PMEM(check_addr));
    passert((uintptr_t) check_addr % CSIZE == 0);
    uintptr_t check_offset = ((uintptr_t) check_addr - PMEM_BASE);
    size_t parity_num_rows = g_layout_hdr->parity_num_rows; // 100

    uintptr_t parity_base = (uintptr_t)g_pavise_pool.addr + g_layout_hdr->parity_offset;

    size_t parity_row_size = g_layout_hdr->parity_row_size;
    void* parity_addr =(void*)(parity_base + check_offset % parity_row_size);

    void* xor_gen_arr[g_layout_hdr->parity_num_rows + 1];
    void* xor_gen_arr_shadow[g_layout_hdr->parity_num_rows + 1];
    for(int i = 0; i < g_layout_hdr->parity_num_rows; i++){
        xor_gen_arr[i] = (void*)(PMEM_BASE + i*parity_row_size + check_offset % parity_row_size);
        xor_gen_arr_shadow[i] = (unsigned char*)xor_gen_arr[i] + SP_DIFF;
        int pos = memcmp_pos(xor_gen_arr[i], ((unsigned char*)xor_gen_arr[i] + SP_DIFF), CSIZE);
        
        //std::cout << "i = " << i << ", addr = " << xor_gen_arr[i] << ", pos = " << pos << std::endl;
    }

    //int corrupted_row = check_offset / parity_row_size;
    //xor_gen_arr[corrupted_row] = parity_addr;
    //xor_gen_arr[g_layout_hdr->parity_num_rows] = check_addr;
    char buf[512];
    xor_gen_arr[g_layout_hdr->parity_num_rows] = buf;
    xor_gen(g_layout_hdr->parity_num_rows+1,CSIZE,xor_gen_arr);

    char buf_shadow[512];
    xor_gen_arr_shadow[g_layout_hdr->parity_num_rows] = buf_shadow;
    xor_gen(g_layout_hdr->parity_num_rows+1,CSIZE,xor_gen_arr_shadow);

    int pos = memcmp_pos(parity_addr, buf, 512);
    std::cout << "pos = " << pos << std::endl;
    //int pos = memcmp_pos(buf_shadow, buf, 512);
    return 0;
}

// recovered data = Parity XOR Data in column
int pavise_recover(void* check_addr){
    passert(WITHIN_PMEM(check_addr));
    passert((uintptr_t) check_addr % CSIZE == 0);
    uintptr_t check_offset = ((uintptr_t) check_addr - PMEM_BASE);
    //dump_memory_region(check_addr, 512);
    //std::cout << "check_offset = " << std::hex << check_offset << std::dec<< std::endl;
    size_t parity_num_rows = g_layout_hdr->parity_num_rows; // 100

    uintptr_t parity_base = (uintptr_t)g_pavise_pool.addr + g_layout_hdr->parity_offset;

    size_t parity_row_size = g_layout_hdr->parity_row_size;
    //std::cout << "PRS = " << parity_row_size<< std::endl;
    void* parity_addr =(void*)(parity_base + check_offset % parity_row_size);

    void* xor_gen_arr[g_layout_hdr->parity_num_rows + 1];
    for(int i = 0; i < g_layout_hdr->parity_num_rows; i++){
        xor_gen_arr[i] = (void*)(PMEM_BASE + i*parity_row_size + check_offset % parity_row_size);
        //std::cout << "i = " << i << ", addr = " << xor_gen_arr[i] << std::endl;
    }
    int corrupted_row = check_offset / parity_row_size;
    //std::cout << "crow = " << corrupted_row << ", addr = " << xor_gen_arr[corrupted_row] << ", check_addr = " << check_addr << std::endl;
    //std::cout << "parity base = " << (void*)parity_base << std::endl;
    xor_gen_arr[corrupted_row] = parity_addr;
    xor_gen_arr[g_layout_hdr->parity_num_rows] = check_addr;
    xor_gen(g_layout_hdr->parity_num_rows+1,CSIZE,xor_gen_arr);
    //dump_memory_region(check_addr, 512);
    return 0;
}
int pavise_verify_recover(void* check_addr, bool force){
    passert(WITHIN_PMEM(check_addr));
    uint64_t key = uaddr_map(check_addr);

    const struct csum_entry* e = GET_CSUM_CONST(key);
    if(check_addr == 0) {
        passert(0);
        // either mapping was not established, or key was invalid
        //std::cout << "pavise_verify_recover: map_uaddr failed, key " << std::hex << key << std::dec << " invalid, skipping\n";
        return 0;
    }
    uint32_t chksum = isal_adler32(0, (const unsigned char*) check_addr, CSIZE);
    //std::cout << "verifying " << check_addr << ", old checksum is " << e->chksum << ", new checksum is " << chksum<< std::endl;
    //std::cout << "key = " << key << ", addr = " << (void*)check_addr << std::endl;
    //passert(0);
    if(e->timestamp == 0xdeadbeef && chksum != e->chksum) {
        //return 1;
        //mismatch_addr.insert((void*) check_addr);
        // comment out mismatch print for naive implementation
        //std::cout << "2Checksum mismatch at " << (void*) check_addr << " detected. Old csum ="
        //    << e->chksum << ", new csum = " << chksum << ". Attempting to recover...\n";
        //std::cout << "!!!!!!mismatch addresses = " << mismatch_addr.size() << std::endl;
        //passert(0);
#ifdef PAVISE_RUNTIME_EVAL
        return 1;
#endif
        /*
        std::cout << "Checksum mismatch at " << (void*) check_addr << " detected. Old csum ="
            << e->chksum << ", new csum = " << chksum << ". Attempting to recover...\n";
        //passert(0);
        pavise_recover(check_addr);
        //verify again after recovery
        chksum = isal_adler32(0, (const unsigned char*) check_addr, CSIZE);
        if(chksum != e->chksum){
            std::cout << "recovery failed\n";
        }
        else{
            std::cout << "recovery success!\n";
        }
        */
#ifdef PAVISE_RUNTIME_EVAL
        if(force) assert(0);
#endif
        if(force || already_computed.find((unsigned char*) check_addr) != already_computed.end()){
            //std::cout << "mismatch\n";
            //return 0;
            int pos = memcmp_pos((char*)check_addr, (char*)((uintptr_t)check_addr + SP_DIFF), CSIZE);
            uint32_t shadow_chksum = isal_adler32(0, (const unsigned char*) ((uintptr_t)check_addr + SP_DIFF), CSIZE);
            void* actual_addr = ((uintptr_t) check_addr + pos);
            //std::cout << "Checksum mismatch at " << (void*) check_addr << " detected. Old csum ="
                //<< e->chksum << ", new csum = " << chksum << ", shadow csum = " << shadow_chksum << ", addr = " << actual_addr << ", pos = " << pos << ". Attempting to recover...\n";
            /*
            for(auto it = already_computed.begin(); it != already_computed.end(); it++){
                std::cout << "already computed: " << (void*)(*it) << std::endl;
            }
            */
            pavise_recover(check_addr);
            //verify again after recovery
            chksum = isal_adler32(0, (const unsigned char*) check_addr, CSIZE);
            if(chksum != e->chksum){
                std::cout << "recovery failed\n";
                rfail++;
                //passert(0);
            }
            else{
                std::cout << "recovery success!\n";
                rsuccess++;
            }
        }
    }
    else{
        //std::cout << "Checksum MATCH at " << (void*) check_addr <<",csum = " << chksum << "\n";
    }
    return 0;
}

void pavise_invalidate(struct csum_entry* e){
    return;
}

void pavise_flush_invalidated(){
}

int hash_func(uint64_t addr, size_t hashmap_size){
    // Knuth's Multiplicative Method
    return ((uint64_t)addr * 2654435761) % hashmap_size;
}  

void pavise_persist_internal(void* addr, size_t len) {
    // Same as flush_clwb_nolog in PMDK, loop through cache lines in the given range
    for( uintptr_t ptr = (uintptr_t) addr & ~(PCACHELINE_SIZE - 1);
            ptr < (uintptr_t) addr + len; ptr += PCACHELINE_SIZE) {
        _mm_clwb((void*)ptr);
        //std::cout << "clwbing " << (void*)ptr << std::endl;
    }
    _mm_sfence();
}

void pavise_flush_internal(void* addr, size_t len){
    // Same as flush_clwb_nolog in PMDK, loop through cache lines in the given range
    for( uintptr_t ptr = (uintptr_t) addr & ~(PCACHELINE_SIZE - 1);
            ptr < (uintptr_t) addr + len; ptr += PCACHELINE_SIZE) {
        _mm_clwb((void*)ptr);
        //_mm_clflushopt((void*)ptr);
    }
    // No drain
}

/* Update checksum for a chunk
 */
void pavise_add_csum_redo (struct redo_header* redo_hdr, struct redo_entry* re,
        uint64_t key,unsigned char* caddr ,unsigned char* log_data){

    // Create shadow copy (to be pushed into csum_shadow
    const struct csum_entry* pm_entry = GET_CSUM_CONST(key);
    struct csum_entry e = *pm_entry;

    /*
    int valid = (e.timestamp) & 1;
    if(e.timestamp!= 0 && e.timestamp != 1) {
        std::cout << "timestamp is corrupted: value is " << e.timestamp << " for key " << e.key << std::endl;
        e.timestamp = valid;
    }
    if(!valid){
        e.key = key;
    }
    */

    e.chksum = isal_adler32(0, (unsigned char*) caddr, CSIZE);
    uintptr_t sp_diff = main_pool_mapped ? SP_DIFF : PMEM_BASE;
    already_computed.insert((unsigned char*)((uintptr_t)caddr - sp_diff));
    //if((uintptr_t)caddr == 0x30000000e00ULL)
    //std::cout << "computing csum : addr = " << (void*)caddr <<", pavise location = " << (void*) pm_entry << ", csum = " << e.chksum<< std::endl;
    //csum_computed.insert((char*)caddr);
    e.caddr = caddr;
    //e.chksum = 0;
    e.timestamp = 0xdeadbeef;

    // Write to redo log
    size_t size = sizeof(e);
    redo_hdr->total_size += size;
    redo_hdr->num_entries++;
    re->uaddr = pm_entry;
    re->size = size;
    memcpy(log_data, &e, size);
}

// Update checksum for a zero chunk
void pavise_add_csum_redo_zero (struct redo_header* redo_hdr, struct redo_entry* re,
        uint64_t key ,unsigned char* log_data){

    // Create shadow copy (to be pushed into csum_shadow
    const struct csum_entry* pm_entry = GET_CSUM_CONST(key);
    struct csum_entry e = *pm_entry;
    e.chksum = 0;
    // Write to redo log
    size_t size = sizeof(e);
    redo_hdr->total_size += size;
    redo_hdr->num_entries++;
    re->uaddr = pm_entry;
    re->size = size;
    memcpy(log_data, &e, size);
}

void pavise_add_parity_redo (
        std::unordered_map<uint64_t, struct csum_entry>& csum_shadow,
        std::vector<std::pair<struct parity_entry*, uintptr_t>>& parity_shadow,
        std::unordered_set<uint64_t>& parity_queue) {

    return;
}

void pavise_add_redo_entry (
        struct redo_header* redo_hdr, struct redo_entry* re, uintptr_t raddr,
        void* saddr, size_t size, unsigned char* log_data) {

    passert(size<65535);
    redo_hdr->total_size += size;
    redo_hdr->num_entries++;
    re->uaddr = raddr;
    re->saddr = (uintptr_t) saddr;
    re->size = size;
    memcpy(log_data, saddr, size);
    return;
}

void pavise_add_redo_memset_entry (
        struct redo_header* redo_hdr, struct redo_entry* re, uintptr_t raddr,
        int val, size_t size,  unsigned char* log_data) {

    redo_hdr->total_size += sizeof(struct redo_memset_entry);
    redo_hdr->num_entries++;
    re->uaddr = raddr;
    re->size = 0;
    struct redo_memset_entry rme = {val,size};
    //std::cout << "adding to redo, size = " << size << "\n";
    memcpy(log_data, &rme, sizeof(struct redo_memset_entry));
    return;
}

uint64_t adst,bdst;
int apfirst = 1;
void pavise_redo_apply(struct redo_header* redo_hdr){
    //clock_t t;
    //t = clock();
    //std::cout << "=============== New TX ======================\n\n";

    struct redo_entry* re = (struct redo_entry*) (redo_hdr + 1);
    unsigned char* log_src = (unsigned char*) redo_hdr + redo_hdr->data_offset;
    void* dst;
    //std::cout << "num entries = " << redo_hdr->num_entries << std::endl;
    for(unsigned i = 0; i < redo_hdr->num_entries; i++){
        if( ((uintptr_t) re[i].uaddr) >= (PAVISE_BASE) && 
                (uintptr_t) re[i].uaddr < PAVISE_BASE + PMEM_BASE ){ // pavise pool address
            dst = (void*)re[i].uaddr;
            memcpy(dst, log_src, re[i].size);
            pavise_flush_internal(dst, re[i].size);
            //std::cout << dst <<std::endl;
            struct csum_entry* e = (struct csum_entry*) log_src;
            //std::cout << "pavise addr, size = " << re[i].size << std::endl;
            //std::cout << "applying checksum to pavise, addr = " <<(void*) e->caddr << ", pavise location = " << dst << std::endl;
            //pavise_memcpy_nt(dst, log_src, re[i].size);
            //std::cout << "pavise persisting addr = " << dst << ", src = " << (void*)log_src << ", size = " << re[i].size << std::endl;
            log_src += re[i].size;
        }
        else { // user pool address
            //dst = (void*) map_uaddr(re[i].uaddr);
            dst = (void*) (re[i].uaddr);
            /*
            if(apfirst){
                adst = 0x50000f00000ULL;
                bdst = 0x10000000000ULL;
                apfirst = 0;
            }
            else{
                adst = (uint64_t)adst + 0x200;
                bdst = (uint64_t)bdst + 0x200;
            }
            */
            passert((uintptr_t)dst >= PMEM_BASE && (uintptr_t)dst  < SHADOW_BASE);
            if(re[i].size == 0){
                //memset
                struct redo_memset_entry* rme = (struct redo_memset_entry*) log_src;
                //std::cout << "applying redo, size = " << rme->size << "\n";
                //if(WITHIN_RANGE_REAL(dst))
                //std::cout << "applying memset redo : addr = " << dst << ", len = " << rme->size << std::endl;
                memset(dst, rme->val, rme->size);
                //pavise_flush_internal(dst, rme->size);
                log_src += sizeof(struct redo_memset_entry);
            }
            else{
                size_t rsize = re[i].size;
                /*
                uintptr_t saddr = re[i].saddr;
                uintptr_t dst_block_aligned = (uintptr_t)dst & CSIZE_MASK;
                uintptr_t saddr_block_aligned = (uintptr_t)saddr & CSIZE_MASK;
                memcpy((void*)dst_block_aligned, (void*) saddr_block_aligned, CSIZE);
                */
                //memcpy(dst, re[i].saddr, rsize); // Copys data form 0x3000... to 0x1000....
                memcpy(dst, log_src, rsize); // Copys data form 0x3000... to 0x1000....
                //if(WITHIN_RANGE_REAL(dst))
                //std::cout << "applying normal redo : addr = " << dst << ", len = " << rsize << std::endl;
                //memcpycnt++;
                memcpysize+=rsize;
                //pavise_memcpy_nt(adst, log_src, re[i].size);
                //int64_t diff = (int64_t) prev_addr - (int64_t)dst;
                //prev_addr = (uintptr_t) dst;
                //std::cout << "******user persisting addr = " << (void*)dst << ", diff w/prev = " << diff << ", size = " << re[i].size << std::endl;
                //std::cout << "******user persisting addr = " << (void*)dst << ", size = " << re[i].size << std::endl;
                log_src += re[i].size;
            }
        }
    }
    PSFENCE();
    //t = clock() - t;
    //memcpytime += ((double)t)/CLOCKS_PER_SEC;
}


void pavise_gen_unique_chunks(
        std::unordered_map<uint64_t, unsigned char*>& chunks,
        std::unordered_map<uint64_t, unsigned char*>& zero_chunks,
        std::unordered_map<uint64_t, unsigned char*>& dirty_cachelines)
{
    uintptr_t base,sp_diff;
    if(main_pool_mapped){
        base = SHADOW_BASE;
        sp_diff = SP_DIFF;
    }
    else{
        base = PART_SHADOW_BASE;
        sp_diff = PMEM_BASE;
    }
    for(auto it = redo_list.begin(); it != redo_list.end(); it++) {
        uintptr_t shadow = std::get<0>(*it) + base;
        uintptr_t real = shadow - sp_diff;
        size_t size = std::get<1>(*it);

        uintptr_t uaddr_start = uaddr_map((void*)real);
        uintptr_t uaddr_end = uaddr_start + size - 1;
        uint64_t chunk_start = uaddr_start & CSIZE_MASK;
        uint64_t chunk_end = uaddr_end & CSIZE_MASK;

        // Loop through covered chunks
        int nchunks = ( chunk_end - chunk_start ) /  CSIZE + 1;
        for(int j = 0; j < nchunks; j++){
            uint64_t key = chunk_start + j*CSIZE;
            unsigned char* check_addr = (unsigned char*) ((shadow & CSIZE_MASK) + j*CSIZE);
            chunks.insert({key, check_addr});
        }

        uint64_t cl_start = uaddr_start & CLSIZE_MASK;
        uint64_t cl_end = uaddr_end & CLSIZE_MASK;

        // Loop through covered cachelines
        /*
        int ncachelines = ( cl_end - cl_start ) /  CLSIZE + 1;
        for(int j = 0; j < ncachelines; j++){
            uint64_t key = cl_start + j*CLSIZE;
            unsigned char* check_addr = (unsigned char*) ((shadow & CLSIZE_MASK) + j*CLSIZE);
            dirty_cachelines.insert({key, check_addr});
        }
        */
    }
    // Do the same for memset operations
    for(auto it = memset_list.begin(); it != memset_list.end(); it++){
        uintptr_t shadow = std::get<0>(*it);
        uintptr_t real = shadow - SP_DIFF;
        size_t size = std::get<1>(*it);
        int val = std::get<2>(*it);
        
        uintptr_t uaddr_start = uaddr_map((void*)real);
        uintptr_t uaddr_end = uaddr_start + size - 1;
        uint64_t chunk_start = uaddr_start & CSIZE_MASK;
        uint64_t chunk_end = uaddr_end & CSIZE_MASK;

        // Loop through covered chunks
        int nchunks = ( chunk_end - chunk_start ) /  CSIZE + 1;
        // If it is a zero memset, we don't need to compute the checksum for the chunks
        // in the middle, because we know the checksum will also be 0
        if (val == 0 && nchunks > 2) {
            for(int j = 1; j < nchunks - 1; j++){
                uint64_t key = chunk_start + j*CSIZE;
                unsigned char* check_addr = (unsigned char*) ((shadow & CSIZE_MASK) + j*CSIZE);
                zero_chunks.insert({key,check_addr});
            }
            //Add first and last chunk to regular chunks
            uint64_t key = chunk_start;
            unsigned char* check_addr = (unsigned char*) ((shadow & CSIZE_MASK));
            chunks.insert({key, check_addr});
            key = chunk_start + (nchunks - 1)*CSIZE;
            check_addr = (unsigned char*) ((shadow & CSIZE_MASK) + (nchunks - 1)*CSIZE);
            chunks.insert({key, check_addr});
        }
        else{
            for(int j = 0; j < nchunks; j++){
                uint64_t key = chunk_start + j*CSIZE;
                unsigned char* check_addr = (unsigned char*) ((shadow & CSIZE_MASK) + j*CSIZE);
                chunks.insert({key, check_addr});
                //std::cout << "memset inserting " << (void*) check_addr << std::endl;
            }
        }

        // Loop through covered cachelines
        // Regardless of whether the memset is a zeroing, they all need to be flushed
        /*
        uint64_t cl_start = uaddr_start & CLSIZE_MASK;
        uint64_t cl_end = uaddr_end & CLSIZE_MASK;

        int ncachelines = ( cl_end - cl_start ) /  CLSIZE + 1;
        for(int j = 0; j < ncachelines; j++){
            uint64_t key = cl_start + j*CLSIZE;
            unsigned char* check_addr = (unsigned char*) ((shadow & CLSIZE_MASK) + j*CLSIZE);
            dirty_cachelines.insert({key, check_addr});
        }
        */
    }
}

void pavise_memcmp_chunks(std::unordered_map<uint64_t, unsigned char*>& chunks){

    std::cout << "=============================================================== begin memcmp\n";
    for(auto it = chunks.begin(); it != chunks.end(); ++it){
        unsigned char* shadow = it->second;
        unsigned char* real = (uintptr_t)shadow - 0x20000000000ULL;
        int cmpres = memcmp_pos(shadow,real,CSIZE); 
        if(cmpres){
            std::cout << "memcmp mismatch at " << (void*) shadow << "\n";
            //passert(0);
        }
    }
    std::cout << "=============================================================== end memcmp\n";

}


//uint32_t num_pages_allocated_tmp = 0;

void pavise_redo_process(){
#ifdef PAVISE_PINTOOL
    std::cout << "=============================================================== begin REDO process\n";
    return;
#endif
    //tx_count++;
    //if(tx_count % 100 != 0) return;
    plock.lock();
    //std::cout << "=============================================================== begin REDO process\n";
    //return;
    //clock_t t;
    //t = clock();
    //std::cout << "redo size = " << redo_list.size() << std::endl;
    // Prepare redo log headers
#ifdef PAVISE_ERR_INJ_EVAL
    if(!first_passed && main_pool_mapped) pavise_verify_all();
#endif

    //printcount++;
    //if(printcount % 1000 == 0){
    //    std::cout << "pages allocated = " << pages_allocated.size() << ", checksums computed = " << csum_computed.size()<<std::endl;
    //}
    
    // treating each pavise_redo_process call as a transaction
    //if (num_pages_allocated_tmp  != pages_allocated.size()) {
    //    std::cout << "!!! tx alloced page!! " << pages_allocated.size() - num_pages_allocated_tmp << " \n";
    //    num_pages_allocated_tmp = pages_allocated.size();
    //}


    std::vector<std::tuple<uintptr_t, uint32_t>>* redo_list_p; //<shadow(offset),size>
    std::vector<std::tuple<uintptr_t, uint32_t>> redo_list_coal; //<shadow(offset),size>
    if(redo_list.size() <= REDO_COAL_THRESHOLD){ // enable coalescing
    //if(false){ // disable coalescing
        redo_list_p = &redo_list_coal;
        // Coalesce addresses to reduce log entries
        std::sort(redo_list.begin(),redo_list.end());
        uintptr_t prev = 0;
        uintptr_t start_addr = 0;
        uint64_t coal_size;
        uint64_t diff_accum = 0;
        for(auto it = redo_list.begin(); it != redo_list.end(); it++) {
            uintptr_t shadow = std::get<0>(*it);
            //std::cout << "redo list: " << shadow << ", size = " << std::get<1>(*it)<< std::endl;
            if(start_addr == 0){
                start_addr = shadow;
                coal_size = std::get<1>(*it);
                prev = shadow;
                continue;
            }
            uint64_t diff = shadow - prev;
            //std::cout << "diff = " << shadow - prev << std::endl;
            if(diff > COAL_THRESHOLD){
                //std::cout << "coal size = " << coal_size << std::endl;
                redo_list_coal.push_back(std::make_tuple(start_addr,coal_size));
                start_addr = shadow;
                coal_size = std::get<1>(*it);
                diff_accum = 0;
            }
            else{
                diff_accum += diff;
                coal_size = std::max(coal_size, diff_accum+ std::get<1>(*it));
                //std::cout << "coal_size = " << coal_size << std::endl;

            }
            prev = shadow;
        }
        redo_list_coal.push_back(std::make_tuple(start_addr,coal_size));

        
        //for(auto it = redo_list_coal.begin(); it != redo_list_coal.end();it++){
        //    uintptr_t shadow = std::get<0>(*it);
        //    //std::cout << "redo list coal: " << shadow << ", size = " << std::get<1>(*it)<< std::endl;
        //}
        
        //std::cout << "redo_list size = " << redo_list.size() << ", coal list size = " << redo_list_coal.size() << std::endl;
    }
    else{
        redo_list_p = &redo_list;
    }

    struct redo_header* redo_hdr = (struct redo_header*) ((uintptr_t)g_pavise_pool.addr + g_layout_hdr->redo_offset);
    redo_hdr->num_entries = 0;
    //int reserved_entries = redo_list.size() + memset_list.size();
    int reserved_entries = redo_list_p->size() + memset_list.size();
    
    //int reserved_entries = redo_array_size;
    struct redo_entry* re = (struct redo_entry*) (redo_hdr + 1);

    // Create list of chunks to update checksum for (no duplicates)
    // key is key, value is check address
    std::unordered_map<uint64_t, unsigned char*> chunks;

    // same as above, except we know all the values are 0
    std::unordered_map<uint64_t, unsigned char*> zero_chunks;

    // Record dirty cachelines
    std::unordered_map<uint64_t, unsigned char*> dirty_cachelines;

    // fill these lists
    pavise_gen_unique_chunks(chunks,zero_chunks,dirty_cachelines);


    reserved_entries += chunks.size() + zero_chunks.size(); // One redo entry for one chksum entry and its orphans
    redo_hdr->data_offset = sizeof(struct redo_header) + reserved_entries * sizeof(struct redo_entry);
    redo_hdr->total_size = redo_hdr->data_offset;
    unsigned char* data = (unsigned char*) re + reserved_entries * sizeof(struct redo_entry);

    int re_cnt = 0;

    // Compute and add csum to redo log. First do zero then regular for correctness
    for(auto it = zero_chunks.begin(); it != zero_chunks.end(); it++) {
        uintptr_t key = it->first;
        pavise_add_csum_redo_zero(redo_hdr,re, key, data);
        data += sizeof(struct csum_entry);
        re++;
        re_cnt++;
        void* shadow = it->second;
        void* real = (void*)((uintptr_t)shadow - SP_DIFF);
        //pavise_update_parity(real, shadow, CSIZE);
    }
    for(auto it = chunks.begin(); it != chunks.end(); it++) {
        unsigned char* check_addr = it->second;
        uintptr_t key = it->first;
        pavise_add_csum_redo(redo_hdr,re, key, check_addr, data);
        data += sizeof(struct csum_entry);
        re++;
        re_cnt++;
        void* shadow = it->second;
        void* real = (void*)((uintptr_t)shadow - SP_DIFF);
        //pavise_update_parity(real, shadow, CSIZE);
    }
    //t = clock() - t;
    //preptime += (double)t / CLOCKS_PER_SEC;
    //t = clock();

    //verify all loaded data before touching any PM
#if defined(PAVISE_VERIFY_LOAD) || defined(PAVISE_VERIFY_STORE)
    pavise_verify();
#endif

    for(auto it = zero_chunks.begin(); it != zero_chunks.end(); it++) {
        void* shadow = it->second;
        void* real = (void*)((uintptr_t)shadow - SP_DIFF);
        pavise_update_parity(real, shadow, CSIZE);
    }
    for(auto it = chunks.begin(); it != chunks.end(); it++) {
        void* shadow = it->second;
        void* real = (void*)((uintptr_t)shadow - SP_DIFF);
        pavise_update_parity(real, shadow, CSIZE);
    }

    // commit user data to redo logs
    // Do memset first, then regular writes, for correctness
    for(auto it = memset_list.begin(); it != memset_list.end(); it++){
        void* shadow = (void*)std::get<0>(*it);
        void* real = (void*)(shadow - SP_DIFF);
        size_t size = std::get<1>(*it);
        int val = std::get<2>(*it);
        pavise_add_redo_memset_entry(redo_hdr, re, real,val,size, data);
        data += sizeof(struct redo_memset_entry); //size;
        re++;
        re_cnt++;
        //pavise_update_parity(real, shadow, size);
    }
    uintptr_t base,sp_diff;
    if(main_pool_mapped){
        base = SHADOW_BASE;
        sp_diff = SP_DIFF;
    }
    else{
        base = PART_SHADOW_BASE;
        sp_diff = PMEM_BASE;
    }
    //for(auto it = redo_list.begin(); it != redo_list.end(); it++) {
    for(auto it = redo_list_p->begin(); it != redo_list_p->end(); it++) {
        void* shadow = (void*)(std::get<0>(*it) + base);
        void* real = (void*)(shadow - sp_diff);
        size_t size = std::get<1>(*it);

        //pavise_add_redo_entry(redo_hdr, re, uaddr_map(real), shadow, size, data);
        // to save space, directly use address rather than uaddr
        pavise_add_redo_entry(redo_hdr, re, real, shadow, size, data);
        data += size;
        re++;
        re_cnt++;
        //free(shadow);
        //pavise_update_parity(real, shadow, size);
    }

    if(re_cnt > reserved_entries){
        std::cout << "re_cnt = " << re_cnt << ", reserved_entries = " << reserved_entries << "\n";
        passert(re_cnt <= reserved_entries);
    }
    //unsigned calculated = redo_list.size() + chunks.size() + memset_list.size() + zero_chunks.size();
    unsigned calculated = redo_list_p->size() + chunks.size() + memset_list.size() + zero_chunks.size();
    //unsigned calculated = redo_array_size + chunks.size();
    passert(redo_hdr->num_entries == calculated);

    //redo_list.clear();
    //memset_list.clear();
    //load_list.clear();
    //already_computed.clear(); // this line dramatically slows down the program?

    /*
    std::thread helper_t(pavise_helper_thread, redo_hdr, std::ref(chunks), std::ref(zero_chunks));
    helper_t.detach();

    return;
    */
    // Compute and store checksum, persist data
    passert(redo_hdr->total_size <= g_layout_hdr->redo_size);
    redo_hdr->chksum = isal_adler32(0, (unsigned char*) (redo_hdr + 1), redo_hdr->total_size - sizeof(struct redo_header));
    pavise_persist_internal(redo_hdr,  redo_hdr->total_size);

    //t = clock() - t;
    //headertime += (double)t / CLOCKS_PER_SEC;

    // Apply redo logs
    pavise_redo_apply(redo_hdr);
    //pavise_memcmp_chunks(chunks);
    //std::cout << "============ verify after redo apply\n";
    //pavise_verify();


    redo_list.clear();
    memset_list.clear();
    load_list.clear();

    //t = clock();
    // Flush dirty cachelines

    //for(auto it = dirty_cachelines.begin(); it != dirty_cachelines.end(); it++) {
    //    unsigned char* check_addr = it->second;
    //    pavise_flush_internal(check_addr, CLSIZE);
    //}

    // Flush dirty chunks
    for(auto it = chunks.begin(); it != chunks.end(); it++) {
        unsigned char* check_addr = it->second;
        pavise_flush_internal(check_addr, CSIZE);
    }
    for(auto it = zero_chunks.begin(); it != zero_chunks.end(); it++) {
        unsigned char* check_addr = it->second;
        pavise_flush_internal(check_addr, CSIZE);
    }
    PSFENCE();

    // discard redo log
    memset(redo_hdr, 0, sizeof(struct redo_header));
    pavise_persist_internal(redo_hdr, sizeof(struct redo_header));

    // Code for freeing pages
    /*
    for(auto it = chunks.begin(); it != chunks.end(); it++){
        unsigned char* check_addr = it->second;
        std::cout << "check addr = " << (void*)check_addr<< std::endl;
        madvise(check_addr,PAGESIZE, MADV_DONTNEED);
    }
    */

    //t = clock() - t;
    //discardtime += (double)t / CLOCKS_PER_SEC;
    //pavise_memcmp_chunks(chunks);
    
    
    /*
    void* sptr = (void*) 0x30000001a00;
    void* rptr = (void*) 0x10000001a00;
    int cmpres = memcmp(sptr,rptr,CSIZE); 
    if(cmpres){
        std::cout << "memcmp mismatch!\n";
        //passert(0);
    }
    */
    
    
    //pavise_parity_column_memcmp((void*)0x100003fe400);
    //std::cout << "end REDO process memcmp = " << cmpres<< "\n";
    //ccount++;
    //std::cout << ccount << std::endl;
    //std::cout << "================================================== end REDO ============" <<  "\n";
    plock.unlock();
    
}


int pavise_isclean(void* ptr){
    return 0;
}

void pavise_mprotect(void* base, uint64_t len, int prot){
    if((uint64_t)base == 0) {
        uintptr_t onlybase = 0;
        uintptr_t onlyend = 0;
        get_only_base(onlybase, onlyend);
        passert(onlyend != 0);
        base = (void*)onlybase;
        len = onlyend - onlybase;
    }
    uint64_t base_aligned = (uint64_t)base & PAGE_MASK;

    uint64_t end_aligned = ((uint64_t)base + len  - 1) & PAGE_MASK;
    uint64_t num_pages = ((end_aligned - base_aligned) >> 12) + 1 ;

    if(mprotect((void*)base_aligned, num_pages * PAGESIZE, prot ) == -1){
        perror("mprotect failed");
        exit(0);
    }
}

// New parity = Old parity XOR (Old data XOR New Data)
void pavise_update_parity(void* old_addr, void* new_addr, size_t size) {
    passert((uintptr_t)old_addr >= PMEM_BASE && (uintptr_t) old_addr <= SHADOW_BASE);
    passert((uintptr_t)new_addr >= SHADOW_BASE && (uintptr_t) new_addr <= PAVISE_BASE);
    if(WITHIN_RANGE(old_addr)){
        //std::cout << "updating parity for " << old_addr << std::endl;
    }
    size += ((uintptr_t)old_addr & 0x1f);
    old_addr = (void*) ((uintptr_t)(old_addr) & XOR_GEN_MASK);
    new_addr = (void*) ((uintptr_t)(new_addr) & XOR_GEN_MASK);
    passert(((uintptr_t)old_addr & 0x1f) == 0);
    uint64_t old_addr_offset = (uintptr_t) old_addr - PMEM_BASE;
    uint64_t new_addr_offset = (uintptr_t) new_addr - SHADOW_BASE;
    passert(old_addr_offset == new_addr_offset);
    
    uintptr_t parity_base = (uintptr_t)g_pavise_pool.addr + g_layout_hdr->parity_offset;

    size_t parity_row_size = g_layout_hdr->parity_row_size;
    void* parity_addr =(void*)(parity_base + old_addr_offset % parity_row_size);
    /*
    std::cout << "parity_base = " <<(void*) parity_base<< ", old_addr_offset = " <<(void*) old_addr_offset 
        << ", mod PRS = " << (void*) (old_addr_offset % parity_row_size)
        << ", parity_addr = " << parity_addr<< std::endl;
        */
    char buf[size];
    memcpy(buf, parity_addr, size);
    passert(((uintptr_t)parity_addr & 0x1f) == 0);

    //void* xor_gen_arr[4] = {old_addr,new_addr,parity_addr,parity_addr};
    void* xor_gen_arr[4] = {old_addr,new_addr,buf,parity_addr};
    int res = xor_gen(4,size,xor_gen_arr);
    if(res) assert(0);
}

void pavise_release_lock(){
    passert(0);
    if(holds_lock){
        holds_lock = false;
        plock.unlock();
    }
}

void dump_memory_region(void* addr, size_t len){
    len += 8 - len % 8;
    assert(len%8 == 0);
    int num_rows = len / 8;
    unsigned char c;
    std::cout << std::hex;
    for(int i = 0; i < num_rows; ++i){
        std::cout << addr << ":   ";
        for(int j = 0; j < 8; ++j){
            c = *((unsigned char*)addr + i*8 + j);
            std::cout << std::setfill('0') << std::setw(2) << (int)c << " ";
        }
        addr = (void*)((unsigned char*)addr + 8);
        std::cout << std::endl;
    }
    std::cout << std::dec;
}
