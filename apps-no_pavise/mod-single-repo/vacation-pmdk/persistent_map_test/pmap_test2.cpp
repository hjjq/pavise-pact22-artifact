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


#include <cassert>
#include <algorithm>
#include <sstream>
#include <map>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

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

class MapTest {
  public:
      persistent_ptr<std::map<long, long>> pmap;

      long get (long key) {
          auto idx = pmap->find(key);
          if (idx != pmap->end())
              return idx->second;
          return -1;
      }

      void put (long key, long value) {
          pmap->insert(std::make_pair(key, value));
          return;
      }

      size_t size () { 
          return pmap->size();
      }
};

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
        transaction::run(pop, [&] {
                tester->pmap = make_persistent<std::map<long, long>>();
                });
        tester->put(1, 1);
        tester->put(2, 2);
        std::cout << "Size of tester:" << tester->size() << std::endl;
    }

    pop.close();
    return 0;
}
