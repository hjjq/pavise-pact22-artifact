#!/bin/bash
#rm -rf /pmem0p1/kevin/pools/*
   
#LD_LIBRARY_PATH="$path/nondebug/:$LD_LIBRARY_PATH"
#$path/benchmarks/pmembench map_insert -n $1 -d 256 -t $2 -f /pmem0p1/kevin/pools/map -T $3
#rpf;
#$PIN_ROOT/pin -t /home/kevin/pavise/pavise_isca22/pin-3.10/../src/pintool/obj-intel64/pintool.so -o nimplog_hmatomic  -- ./pmembench map_insert -n 10000 -d 256 -f /pmem0p1/kevin/pools/map -T hashmap_atomic
#$PIN_ROOT/pin -t /home/kevin/pavise/pavise_isca22/pin-3.10/../src/pintool/obj-intel64/pintool.so -o implog_hmatomic -rec 1 -- ./pmembench map_insert -n 10000 -d 256 -f /pmem0p1/kevin/pools/map -T hashmap_atomic
#rpf;
#$PIN_ROOT/pin -t /home/kevin/pavise/pavise_isca22/pin-3.10/../src/pintool/obj-intel64/pintool.so -o nimplog_hmtx  -- ./pmembench map_insert -n 10000 -d 256 -f /pmem0p1/kevin/pools/map -T hashmap_tx 
#$PIN_ROOT/pin -t /home/kevin/pavise/pavise_isca22/pin-3.10/../src/pintool/obj-intel64/pintool.so -o implog_hmtx -rec 1 -- ./pmembench map_insert -n 10000 -d 256 -f /pmem0p1/kevin/pools/map -T hashmap_tx
#rpf;
#$PIN_ROOT/pin -t /home/kevin/pavise/pavise_isca22/pin-3.10/../src/pintool/obj-intel64/pintool.so -o nimplog_ctree  -- ./pmembench map_insert -n 10000 -d 256 -f /pmem0p1/kevin/pools/map -T ctree
#$PIN_ROOT/pin -t /home/kevin/pavise/pavise_isca22/pin-3.10/../src/pintool/obj-intel64/pintool.so -o implog_ctree -rec 1 -- ./pmembench map_insert -n 10000 -d 256 -f /pmem0p1/kevin/pools/map -T ctree
#rpf;
#$PIN_ROOT/pin -t /home/kevin/pavise/pavise_isca22/pin-3.10/../src/pintool/obj-intel64/pintool.so -o nimplog_btree  -- ./pmembench map_insert -n 10000 -d 256 -f /pmem0p1/kevin/pools/map -T btree
#$PIN_ROOT/pin -t /home/kevin/pavise/pavise_isca22/pin-3.10/../src/pintool/obj-intel64/pintool.so -o implog_btree -rec 1 -- ./pmembench map_insert -n 10000 -d 256 -f /pmem0p1/kevin/pools/map -T btree
rpf;
$PIN_ROOT/pin -t /home/kevin/pavise/pavise_isca22/pin-3.10/../src/pintool/obj-intel64/pintool.so -o nimplog_rtree  -- ./pmembench map_insert -n 10000 -d 256 -f /pmem0p1/kevin/pools/map -T rtree
$PIN_ROOT/pin -t /home/kevin/pavise/pavise_isca22/pin-3.10/../src/pintool/obj-intel64/pintool.so -o implog_rtree -rec 1 -- ./pmembench map_insert -n 10000 -d 256 -f /pmem0p1/kevin/pools/map -T rtree
rpf;
#$PIN_ROOT/pin -t /home/kevin/pavise/pavise_isca22/pin-3.10/../src/pintool/obj-intel64/pintool.so -o nimplog_rbtree  -- ./pmembench map_insert -n 10000 -d 256 -f /pmem0p1/kevin/pools/map -T rbtree
#$PIN_ROOT/pin -t /home/kevin/pavise/pavise_isca22/pin-3.10/../src/pintool/obj-intel64/pintool.so -o implog_rbtree -rec 1 -- ./pmembench map_insert -n 10000 -d 256 -f /pmem0p1/kevin/pools/map -T rbtree
#rpf;
#rm -rf /pmem0p1/kevin/pools/*

echo $LD_LIBRARY_PATH
