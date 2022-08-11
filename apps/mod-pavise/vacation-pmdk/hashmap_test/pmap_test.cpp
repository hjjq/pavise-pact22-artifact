/* =============================================================================
 *
 * vacation.cpp
 *
 * =============================================================================
 *
 * Author: Swapnil Haria 
 * This code is based on the vacation benchmark from the STAMP suite.
 * Similar to vacation in the WHISPER suite, manager object is persistent
 * but clients are not persistent.
 * TODO: make clients persistent?
 * =============================================================================
 */

extern "C"
{
#include <examples/libpmemobj/hashmap/hashmap_tx.h>
}

#include <cassert>
#include <algorithm>
#include <sstream>
#include <map>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

#include <libpmemobj.h>
#include <libpmemobj++/allocator.hpp>
#include <libpmemobj++/persistent_ptr.hpp>
#include <libpmemobj++/make_persistent.hpp>
#include <libpmemobj++/make_persistent_atomic.hpp>
#include <libpmemobj++/p.hpp>
#include <libpmemobj++/transaction.hpp>

using pmem::obj::make_persistent;
using pmem::obj::make_persistent_atomic;
using pmem::obj::p;
using pmem::obj::persistent_ptr;
using pmem::obj::pool_base;
using pmem::obj::transaction;
using pmem::obj::pool;

#define LAYOUT "pmap_test"

#define CREATE_MODE_RW (S_IWUSR | S_IRUSR)
#define OBJ_TYPE_NUM 1

template <typename K, typename V>
class PMDKMap {
  public:
    void init (pool_base &pop) {
        pop_ = pop.handle();
    }

    bool createMap () {
        if(POBJ_ZALLOC(pop_, &map_, struct hashmap_tx, sizeof(struct hashmap_tx))) {
            std::cerr << "zalloc failed with " << pmemobj_errormsg() << std::endl;
            return false;
        }
        std::cout << "map created at :" << (void*) &map_ << std::endl;
        if (hm_tx_create(pop_, &map_, NULL)) {
            perror("map_new");
            return false;
        } 
        return true;
    }

    void put(K key, V value) {
        PMEMoid value_ptr;
        pmemobj_zalloc(pop_, &value_ptr, sizeof(V), OBJ_TYPE_NUM);
        *(V*)(pmemobj_direct(value_ptr)) = value;
        hm_tx_insert(pop_, map_, key, value_ptr);
    }

    V get(K key) {
        return *(V*)(pmemobj_direct(hm_tx_get(pop_, map_, key)));
    }

    size_t size() {
        return hm_tx_count(pop_, map_);
    }

  private: 
    //persistent_ptr<struct hashmap_tx> map_;
    TOID(struct hashmap_tx) map_; 
    PMEMobjpool* pop_;
};

using MapTest = PMDKMap<uint64_t, uint64_t>;
#define DEREF(a) *(uint64_t*)(pmemobj_direct(a))

bool BackingFileExists(const char* file) {
    return (access(file, F_OK) == 0);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Error! Correct Usage: ./vacation backing-file <vacation options>\n";
        return 0;
    }
    int opt;
    const char* path = argv[1];
   
    pool<MapTest> pop;
    bool recover = BackingFileExists(path);
    if (recover) {
        std::cout << "Recovering from backing file\n"; 
        pop = pool<MapTest>::open(path, LAYOUT);
        auto tester = pop.root();
        tester->init(pop);
        auto size = (long) tester->size();
        std::cout << "Size of tester:" << size << std::endl;
        std::cout << size << ":" << tester->get(size) << std::endl;
        tester->put(size+1, size+1);
        std::cout << "Size of tester:" << tester->size() << std::endl;
    } else {
        std::cout << "Initializing backing file\n"; 
        pop = pool<MapTest>::create(
                path, LAYOUT, PMEMOBJ_MIN_POOL, CREATE_MODE_RW);
        auto tester = pop.root();
        tester->init(pop);
        if (!tester->createMap()) {
            std::cerr << "Failed to create map" << std::endl;
        }
        tester->put(1, 1);
        tester->put(2, 2);
        std::cout << "Size of tester:" << tester->size() << std::endl;
    }

    pop.close();
    return 0;
}
