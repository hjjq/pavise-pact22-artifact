#!/bin/bash
rm -rf /mnt/ext4-pmem18/memcached*
export PMEM_NO_MOVNT=0
export PMEM_IS_PMEM_FORCE=1
MEMCACHED_DIR=/home/swapnilh/mnemosyne-gcc/usermode/
#export LD_LIBRARY_PATH=${MEMCACHED_DIR}/library/:/home/swapnilh/pmdk-stuff/pmdk/src/nondebug/:/home/swapnilh/pmdk-stuff/pmdk/src/examples/libpmemobj/hashmap/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=${MEMCACHED_DIR}/library/:/home/swapnilh/pmdk/src/nondebug/:/home/swapnilh/pmdk-stuff/pmdk/src/examples/libpmemobj/hashmap/:$LD_LIBRARY_PATH
ldd build/memcached
taskset 0x100 ./build/memcached /mnt/ext4-pmem18/memcached -u root -p 11211 -l 127.0.0.1 &
sleep 2
${MEMCACHED_DIR}/bench/memcached/memslap -s 127.0.0.1:11211 -c 4 -x 100000 -T 4 -X 512 -F ${MEMCACHED_DIR}/run.cnf -d 1
pkill memcached
