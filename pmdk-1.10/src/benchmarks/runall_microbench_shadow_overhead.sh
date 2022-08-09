#!/bin/bash

dir_name="shadow_10k_blacklist"
csv_name="shadow_10k_blacklist"
iters=1
let iters_minus_1=$iters-1 # minus 1 because we start from 0
mkdir -p $dir_name
echo "benchmark,elapsed time in seconds" > ${dir_name}/${csv_name}.csv
# Iterate the string variable using for loop
for bench in hashmap_atomic hashmap_tx ctree btree rbtree rtree; do
    # run each bench multiple times
    for iter in $(seq 0 $iters_minus_1); do
        echo Running $bench the $iter th time
        rpf
        ./pmembench map_insert -n 10000 -d 256 -f /pmem0p1/kevin/pools/map -T ${bench} > ${dir_name}/${bench}_log${iter} 
    done
done

for bench in hashmap_atomic hashmap_tx ctree btree rbtree rtree; do
    for iter in $(seq 0 $iters_minus_1); do
        # grab the number of pages allocated from benchmark output log.
        # the two rev commands are for finding the last field using cut.
        # example line: pages allocated = 942, checksums computed = 0
        num_pages=$(cat ${dir_name}/${bench}_log${iter} | grep "pages allocated" | cut -d"," -f1 | rev | cut -d" " -f1 | rev)
        # check if there are mismatches
        if cat ${dir_name}/${bench}_log${iter} | grep -q "Checksum mismatch" ; then
            echo "found mismatch in file ${dir_name}/${bench}_log${iter}"
            echo $bench,ERR_MISMATCH >> ${dir_name}/${csv_name}.csv
        else
            # no mismatches     
            echo $bench,$num_pages >> ${dir_name}/${csv_name}.csv
        fi  

    done
done

