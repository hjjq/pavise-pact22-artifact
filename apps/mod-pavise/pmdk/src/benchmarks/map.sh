#!/bin/bash

DATASIZE=64
OPERATIONS=1000000

init_environment () {
    export PMEM_IS_PMEM_FORCE=1
    source ../source.sh
}

run_benchmark () {
    rm -rf /mnt/ext4-pmem22/map
    echo "Executing $1:$2"
    sleep 2
    CMD="taskset 0x100 ./pmembench $1 -f /mnt/ext4-pmem22/map -d $DATASIZE -n $OPERATIONS -T $2"
    echo $CMD
    eval $CMD
}

validate_benchmark () {
    rm -rf /mnt/ext4-pmem22/map
    echo "Validating $1:$2"
    sleep 2
    CMD="valgrind --tool=lackey --flush-counts=yes ./pmembench $1 -f /mnt/ext4-pmem22/map -d $DATASIZE -n 10 -T $2"
    echo $CMD
    eval $CMD
}

init_environment
echo "Validating tx and non-tx implementations first"
run_benchmark map_insert rbtree
run_benchmark map_insert btree
run_benchmark map_insert hashmap_tx
run_benchmark map_insert rbtree_notx
run_benchmark map_insert btree_notx
run_benchmark map_insert hashmap_notx


run_benchmark map_remove rbtree
run_benchmark map_remove btree
run_benchmark map_remove hashmap_tx
run_benchmark map_remove rbtree_notx
run_benchmark map_remove btree_notx
run_benchmark map_remove hashmap_notx


run_benchmark map_get rbtree
run_benchmark map_get btree
run_benchmark map_get hashmap_tx
run_benchmark map_get rbtree_notx
run_benchmark map_get btree_notx
run_benchmark map_get hashmap_notx
