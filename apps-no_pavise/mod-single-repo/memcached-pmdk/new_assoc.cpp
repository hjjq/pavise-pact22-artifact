extern "C" {
  #include <memcached.h>
}

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <time.h>
#include <assert.h>
#ifdef TX
#include <map.hpp>
#else
#include <map-notx.hpp>
#endif
#include <libpmemobj.h>
#include <libpmemobj++/pool.hpp>
#include <libpmemobj++/persistent_ptr.hpp>

using pmem::obj::pool;
using pmem::obj::pool_base;
using pmem::obj::persistent_ptr;
using MAP_T = PMDKMap<KEY_T, item*>;
static persistent_ptr<MAP_T> primary_hashtable;
static PMEMobjpool* curr_pool;

#define LAYOUT "memcached"

#define CREATE_MODE_RW (S_IWUSR | S_IRUSR)

#define POOL_SIZE 8*1024UL*1024*1024 // 8GB

PMEMobjpool* assoc_init(bool recover, const char* path) {
    pool<MAP_T> pop;
    if (recover) {
        pop = pool<MAP_T>::open(path, LAYOUT);
        primary_hashtable = pop.root();
        //primary_hashtable = (MAP_T*) D_RW(toid_map); 
        fprintf(stderr, "***************************************************\n");
        fprintf(stderr, "  Using hash-table from PREVIOUS incarnation \n");
        fprintf(stderr, "  primary_hashtable = %p\n", (void*) pmemobj_direct(primary_hashtable.raw()));
        fprintf(stderr, "***************************************************\n");
    } else {
        pop = pool<MAP_T>::create(path, LAYOUT, POOL_SIZE, CREATE_MODE_RW);
        primary_hashtable = pop.root();
        primary_hashtable->init(pop.handle());
        //TOID(void) *toid_map;
        //POBJ_ALLOC(pop, toid_map, MAP_T, sizeof(PMDKMap*), NULL, NULL);
        //primary_hashtable = (MAP_T*) D_RW(*toid_map); 
        //primary_hashtable->init(pop);
        fprintf(stderr, "***************************************************\n");
        fprintf(stderr, "  Allocated hash-table in CURRENT incarnation \n");
        fprintf(stderr, "  primary_hashtable = %p\n", (void*) pmemobj_direct(primary_hashtable.raw()));
        fprintf(stderr, "***************************************************\n");
    }
    curr_pool = pop.handle();
    return curr_pool;
}

KEY_T convertKey(const char *key, const size_t nkey) {
    char key_buf[16];
    assert(nkey == 16);
    memcpy(&key_buf, key, nkey);
    return atol(key_buf);
}


item* assoc_find(KEY_T key) {
    item** found_item = primary_hashtable->find(curr_pool, key);
    if (found_item == nullptr)
        return 0;
    return *found_item;
}

item* assoc_find(const char *key, const size_t nkey) {
    return assoc_find(convertKey(key, nkey));
}

/* Note: this isn't an assoc_update.  The key must not already exist to call this */
int assoc_insert(item *it) {
    assert(it->nkey == 16);
    auto key = convertKey(ITEM_key(it), it->nkey);
    assert(assoc_find(key) == 0);  // shouldn't have duplicately named things defined
    primary_hashtable->insert(curr_pool, key, it);
    return 1;
}

void assoc_delete(const char *key, const size_t nkey) {
    // We don't check to see if item is present as callers dont call
    // delete if they dont find the item!
    assert(nkey == 16);
    auto uint_key = convertKey(key, nkey);
    primary_hashtable->erase(curr_pool, uint_key);
}
