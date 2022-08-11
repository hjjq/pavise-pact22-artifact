#!/bin/bash

DATASIZE=64
OPERATIONS=10000
MAP_OPERATIONS=( map_insert map_remove map_get )
MAP_TYPES=( rbtree btree hashmap_tx ) #rbtree_notx btree_notx hashmap_notx )
VALGRIND="valgrind --tool=lackey --flush-counts=yes"

init_environment () {
    export PMEM_IS_PMEM_FORCE=1
    source source.sh
}

valgrind_map() {
    rm -rf /mnt/ext4-pmem22/map
    echo "Validating $1:$2"
    sleep 2
    CMD="$VALGRIND ./benchmarks/pmembench $1 -f /mnt/ext4-pmem22/map -d $DATASIZE -n $OPERATIONS -T $2"
    echo $CMD
    eval $CMD
}

valgrind_list () {
    rm -rf /mnt/ext4-pmem22/list.pool 
    ./tools/pmempool/pmempool create obj /mnt/ext4-pmem22/list.pool --layout list --size 1GB
    echo "List INSERT"
    $VALGRIND  ./examples/libpmemobj/linkedlist/$1 /mnt/ext4-pmem22/list.pool $OPERATIONS insert h
    sleep 2
    echo "List REMOVE"
    $VALGRIND  ./examples/libpmemobj/linkedlist/$1 /mnt/ext4-pmem22/list.pool $OPERATIONS remove
}

valgrind_queue () {
    rm -rf /mnt/ext4-pmem22/queue.pool 
    ./tools/pmempool/pmempool create obj /mnt/ext4-pmem22/queue.pool --layout queue --size 1GB
    ./examples/libpmemobj/queue/$1 /mnt/ext4-pmem22/queue.pool 1 new $OPERATIONS 
    echo "Queue ENQUEUE"
    $VALGRIND ./examples/libpmemobj/queue/$1 /mnt/ext4-pmem22/queue.pool $OPERATIONS enqueue hello
    sleep 2
    echo "Queue DEQUEUE"
    $VALGRIND ./examples/libpmemobj/queue/$1 /mnt/ext4-pmem22/queue.pool $OPERATIONS dequeue
}

init_environment
echo " ************** RUNNING MAP BENCHMARKS ********************"
for impl in "${MAP_TYPES[@]}"
do
    for op in "${MAP_OPERATIONS[@]}"
    do
        valgrind_map $op $impl
        echo ""
        echo ""
    done
done

echo " ************** RUNNING LIST BENCHMARKS ********************"
valgrind_list fifo

echo " ************** RUNNING QUEUE BENCHMARKS ********************"
valgrind_queue queue
