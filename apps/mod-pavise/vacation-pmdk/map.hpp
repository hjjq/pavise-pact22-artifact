#pragma once

extern "C"
{
#include <hashmap_tx.h>
}
#include "pavise_interface.h" // PAVISE_EDIT
#include <libpmemobj++/allocator.hpp>
#include <libpmemobj++/persistent_ptr.hpp>
#include <libpmemobj++/make_persistent.hpp>
#include <libpmemobj++/make_persistent_atomic.hpp>
#include <libpmemobj++/p.hpp>
#include <libpmemobj++/transaction.hpp>

using pmem::obj::pool_base;

#define OBJ_TYPE_NUM 1004

// key has to be of type uint64_t
template <typename K, typename V>
class PMDKMap {
  public:
    void init (pool_base &pop) {
//        if (TOID_IS_NULL(map_ )) {
            createMap(pop.handle());
//        }
    }

    bool createMap (PMEMobjpool* pop) {
        if(POBJ_ZALLOC(pop, &map_, struct hashmap_tx, sizeof(struct hashmap_tx))) {
            std::cerr << "zalloc failed with " << pmemobj_errormsg() << std::endl;
            return false;
        }
        if (hm_tx_create(pop, &map_, NULL)) {
            std::cerr << "hm_tx_create failed with " << pmemobj_errormsg() << std::endl;
            perror("map_new");
            return false;
        }
        return true;
    }

    void insert(PMEMobjpool* pop, K key, V value) {
        PMEMoid value_ptr;
        pmemobj_zalloc(pop, &value_ptr, sizeof(V), OBJ_TYPE_NUM);
        *(V*)(pmemobj_direct(value_ptr)) = value;
        hm_tx_insert(pop, map_, key, value_ptr);
    }

    V find(PMEMobjpool* pop, K key) {
        PMEMoid value_ptr = hm_tx_get(pop, map_, key);
        if (OID_IS_NULL(value_ptr)) {
            return V();
        }
        return *(V*)(pmemobj_direct(value_ptr));
    }

    bool erase(PMEMobjpool* pop, K key) {
        auto result = hm_tx_remove(pop, map_, key);
        if (OID_IS_NULL(result))
            return false;
        return true;
    }

    size_t size(PMEMobjpool* pop) {
        return hm_tx_count(pop, map_);
    }

    void print() {
//        hm_tx_cmd(pop, map_, HASHMAP_CMD_DEBUG, (uint64_t)stdout);
    }


    void ForEach(PMEMobjpool* pop, int (*cb) (K, PMEMoid, void*), void* arg) {
        hm_tx_foreach(pop, map_, cb, arg);
    } 

  private: 
    //persistent_ptr<struct hashmap_tx> map_;
    TOID(struct hashmap_tx) map_; 
};
