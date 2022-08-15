#pragma once

extern "C"
{
#include <examples/libpmemobj/hashmap/hashmap_tx.h>
#include <examples/libpmemobj/map/map.h>
}

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

template <typename K, typename V>
class PMDKMap {
  public:
    PMDKMap(pool_base &pop) : pop_(pop) {
        make_persistent_atomic<struct hashmap_tx>(pop_, map_);
    }

    void put(K key, V value) {
        hm_tx_insert(pop_, *map_, key, value);
    }

    V get(K key) {
        return hm_tx_get(pop_, *map_, key);
    }

  private: 
    persistent_ptr<struct hashmap_tx> map_;
    pool_base pop_;
}
