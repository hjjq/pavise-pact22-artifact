#!/bin/bash
#rm -rf /pmem0p1/pmdk/*
   
#LD_LIBRARY_PATH="$path/nondebug/:$LD_LIBRARY_PATH"
#$path/benchmarks/pmembench map_insert -n $1 -d 256 -t $2 -f /pmem0p1/pmdk/map -T $3
rpf;
echo "============ running hashmap_atomic"
./pmembench map_insert -n 1000000 -d 256 -f /pmem0p1/pmdk/map -T hashmap_atomic
rpf;
echo "============ running hashmap_tx"
./pmembench map_insert -n 1000000 -d 256 -f /pmem0p1/pmdk/map -T hashmap_tx 
rpf;
echo "============ running ctree"
./pmembench map_insert -n 1000000 -d 256 -f /pmem0p1/pmdk/map -T ctree
rpf;
echo "============ running btree"
./pmembench map_insert -n 1000000 -d 256 -f /pmem0p1/pmdk/map -T btree
rpf;
echo "============ running rbtree"
./pmembench map_insert -n 1000000 -d 256 -f /pmem0p1/pmdk/map -T rbtree
rpf;
echo "============ running rtree"
./pmembench map_insert -n 1000000 -d 256 -f /pmem0p1/pmdk/map -T rtree
rpf;
#rm -rf /pmem0p1/pmdk/*

echo $LD_LIBRARY_PATH
