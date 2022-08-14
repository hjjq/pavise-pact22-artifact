#pragma once

extern "C"
{
#include <hashmap_notx.h>
}

#include <libpmemobj++/allocator.hpp>
#include <libpmemobj++/persistent_ptr.hpp>
#include <libpmemobj++/make_persistent.hpp>
#include <libpmemobj++/make_persistent_atomic.hpp>
#include <libpmemobj++/p.hpp>
#include <libpmemobj++/transaction.hpp>


#define OBJ_TYPE_NUM 1

// key has to be of type uint64_t
template <typename K, typename V>
class PMDKMap {
  public:
    void init (PMEMobjpool* pop) {
        if (TOID_IS_NULL(map_ ))
            createMap(pop);
    }

    bool createMap (PMEMobjpool* pop) {
        if(POBJ_ZALLOC(pop, &map_, struct hashmap_notx, sizeof(struct hashmap_notx))) {
            std::cerr << "zalloc failed with " << pmemobj_errormsg() << std::endl;
            return false;
        }
        if (hm_notx_create(pop, &map_, NULL)) {
            perror("map_new");
            return false;
        } 
        return true;
    }

    void insert(PMEMobjpool* pop, K key, V value) {
        PMEMoid value_ptr;
        pmemobj_zalloc(pop, &value_ptr, sizeof(V), OBJ_TYPE_NUM);
        *(V*)(pmemobj_direct(value_ptr)) = value;
        hm_notx_insert(pop, map_, key, value_ptr);
    }

    V* find(PMEMobjpool* pop, K key) {
        PMEMoid value_ptr = hm_notx_get(pop, map_, key);
        if (OID_IS_NULL(value_ptr)) {
            return nullptr;
        }
        return (V*)(pmemobj_direct(value_ptr));
    }

    bool erase(PMEMobjpool* pop, K key) {
        auto result = hm_notx_remove(pop, map_, key);
        if (OID_IS_NULL(result))
            return false;
        return true;
    }

    size_t size(PMEMobjpool* pop) {
        return hm_notx_count(pop, map_);
    }

    void print() {
//        hm_notx_cmd(pop, map_, HASHMAP_CMD_DEBUG, (uint64_t)stdout);
    }


    void ForEach(PMEMobjpool* pop, int (*cb) (K, PMEMoid, void*), void* arg) {
        hm_notx_foreach(pop, map_, cb, arg);
    } 

  private: 
    //persistent_ptr<struct hashmap_notx> map_;
    TOID(struct hashmap_notx) map_; 
};
