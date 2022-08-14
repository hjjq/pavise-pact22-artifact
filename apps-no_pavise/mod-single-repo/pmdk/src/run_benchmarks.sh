#!/bin/bash

DATASIZE=64
OPERATIONS=1000000
MAP_OPERATIONS=( map_insert map_remove map_get )
MAP_TX_TYPES=( rbtree btree hashmap_tx )
MAP_NOTX_TYPES=( rbtree_notx btree_notx hashmap_notx )

init_environment () {
    export PMEM_IS_PMEM_FORCE=1
    export PMEM_NO_MOVNT=1
    source source.sh
}

run_map_benchmark () {
    rm -rf /mnt/ext4-pmem22/map
    echo "Executing $1:$2"
    sleep 2
    CMD="taskset 0x100 ./benchmarks/pmembench $1 -f /mnt/ext4-pmem22/map -d $DATASIZE -n $OPERATIONS -T $2"
    echo $CMD
    eval $CMD
}

validate_benchmark () {
    rm -rf /mnt/ext4-pmem22/map
    echo "Validating $1:$2"
    sleep 2
    CMD="valgrind --tool=lackey --flush-counts=yes ./benchmarks/pmembench $1 -f /mnt/ext4-pmem22/map -d $DATASIZE -n 10 -T $2"
    echo $CMD
    eval $CMD
}

run_list_benchmark () {
    rm -rf /mnt/ext4-pmem22/list.pool 
    ./tools/pmempool/pmempool create obj /mnt/ext4-pmem22/list.pool --layout list --size 1GB
    taskset 0x100 ./examples/libpmemobj/linkedlist/$1 /mnt/ext4-pmem22/list.pool 1000000 insert h
    #taskset 0x100 ./examples/libpmemobj/linkedlist/$1 /mnt/ext4-pmem22/list.pool 1 print 
    sleep 2
    taskset 0x100 ./examples/libpmemobj/linkedlist/$1 /mnt/ext4-pmem22/list.pool 1000000 remove
}

run_queue_benchmark () {
    rm -rf /mnt/ext4-pmem22/queue.pool 
    ./tools/pmempool/pmempool create obj /mnt/ext4-pmem22/queue.pool --layout queue --size 1GB
    ./examples/libpmemobj/queue/$1 /mnt/ext4-pmem22/queue.pool 1 new 1000000
    taskset 0x100 ./examples/libpmemobj/queue/$1 /mnt/ext4-pmem22/queue.pool 1000000 enqueue hello
    sleep 2
    taskset 0x100 ./examples/libpmemobj/queue/$1 /mnt/ext4-pmem22/queue.pool 1000000 dequeue
}

{
export PMEM_NO_FLUSH=0
export PMEM_NO_FENCE=0
init_environment
echo " ************** RUNNING MAP BENCHMARKS ********************"
for impl in "${MAP_TX_TYPES[@]}"
do
    for op in "${MAP_OPERATIONS[@]}"
    do
        run_map_benchmark $op $impl
        echo ""
        echo ""
    done
done

echo " ************** RUNNING LIST BENCHMARKS ********************"
run_list_benchmark fifo

echo " ************** RUNNING QUEUE BENCHMARKS ********************"
run_queue_benchmark queue
} > baseline.log 2>baseline.log


{
export PMEM_NO_FLUSH=1
export PMEM_NO_FENCE=1
echo " ************** RUNNING MAP BENCHMARKS ********************"
for impl in "${MAP_TX_TYPES[@]}"
do
    for op in "${MAP_OPERATIONS[@]}"
    do
        run_map_benchmark $op $impl
        echo ""
        echo ""
    done
done

echo " ************** RUNNING LIST BENCHMARKS ********************"
run_list_benchmark fifo

echo " ************** RUNNING QUEUE BENCHMARKS ********************"
run_queue_benchmark queue
} > no_flush_no_fence.log 2>no_flush_no_fence.log


{
export PMEM_NO_FLUSH=1
export PMEM_NO_FENCE=1
echo " ************** RUNNING MAP BENCHMARKS ********************"
for impl in "${MAP_NOTX_TYPES[@]}"
do
    for op in "${MAP_OPERATIONS[@]}"
    do
        run_map_benchmark $op $impl
        echo ""
        echo ""
    done
done

echo " ************** RUNNING LIST BENCHMARKS ********************"
run_list_benchmark fifo_notx

echo " ************** RUNNING QUEUE BENCHMARKS ********************"
run_queue_benchmark queue_notx
} > no_tx.log 2>no_tx.log
