#!/bin/bash


# make sure to change the make target accordingly based on the type of evaluation: 
# (make a_err_inj_eval|a_runtime_eval)

dir_name="pact22_artifact_eval_repro"
csv_name="pact22_artifact_eval_repro"
iters=1
let iters_minus_1=$iters-1 # minus 1 because we start from 0

mkdir -p $dir_name
echo "benchmark,elapsed time in seconds" > ${dir_name}/${csv_name}.csv
# Iterate the string variable using for loop
for bench in hashmap_atomic hashmap_tx ctree btree rbtree rtree; do
    # run each bench multiple times
    for iter in $(seq 0 $iters_minus_1); do
        echo Running benchmark $bench for the $iter th time
        #rpf
        rm -rf /pmem0p1/kevin/pools/*
        ./pmembench map_insert -n 1000000 -d 256 -f /pmem0p1/kevin/pools/map -T ${bench} > ${dir_name}/${bench}_log${iter} 
    done
done

for bench in hashmap_atomic hashmap_tx ctree btree rbtree rtree; do
    for iter in $(seq 0 $iters_minus_1); do
        # grab the elapsed time from benchmark output log: first number on the last line
        time=$(cat ${dir_name}/${bench}_log${iter} | tail -n 1 | cut -d";" -f1)
        # check if there are mismatches
        if cat ${dir_name}/${bench}_log${iter} | grep -q "Checksum mismatch" ; then
            echo "found mismatch in file ${dir_name}/${bench}_log${iter}"
            echo $bench,ERR_MISMATCH >> ${dir_name}/${csv_name}.csv
        else
            # no mismatches     
            echo $bench,$time >> ${dir_name}/${csv_name}.csv
        fi  

    done
done

