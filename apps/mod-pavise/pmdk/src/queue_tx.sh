#!/bin/bash
shopt -s expand_aliases
source ~/.bashrc
source source.sh
rm -rf /mnt/ext4-pmem22/queue.pool 
./tools/pmempool/pmempool create obj /mnt/ext4-pmem22/queue.pool --layout queue --size 1GB
./examples/libpmemobj/queue/queue /mnt/ext4-pmem22/queue.pool 1 new 1000000
./examples/libpmemobj/queue/queue /mnt/ext4-pmem22/queue.pool 1000000 enqueue hello
sleep 2
taskset 0x100 ./examples/libpmemobj/queue/queue /mnt/ext4-pmem22/queue.pool 1000000 dequeue
