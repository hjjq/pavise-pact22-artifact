#!/bin/bash


# make sure to change the make target accordingly based on the type of evaluation: 
# (make a_err_inj_eval|a_runtime_eval)

dir_name="pact22_artifact_eval_repro"
csv_name="pact22_artifact_eval_repro"
iters=1
let iters_minus_1=$iters-1 # minus 1 because we start from 0

#numrows_list=(1 10 20 100 1000 10000 100000)
numrows_list=(100)
mkdir -p $dir_name
echo "num_rows,benchmark,elapsed time in seconds" > ${dir_name}/${csv_name}.csv
# Iterate the string variable using for loop
for numrows in ${numrows_list[@]}; do
    # modify PARITY_NUM_ROWS in source header
    sed -i "s/#define PARITY_NUM_ROWS.*/#define PARITY_NUM_ROWS ${numrows}/" $PAVISE_ROOT/include/pavise.hpp
    # remake pavise and return to benchmark directory
    cd $PAVISE_ROOT
    #make a_err_inj_eval
    make a_runtime_eval
    #make b_runtime_eval
    cd $PAVISE_ROOT/pmdk-1.10/src/benchmarks 
    for bench in hashmap_atomic hashmap_tx ctree btree rbtree rtree; do
    #for bench in rtree; do
        # run each bench multiple times
        for iter in $(seq 0 $iters_minus_1); do
            echo Running num rows $numrows, benchmark $bench for the $iter th time
            rpf
            ./pmembench map_insert -n 1000000 -d 256 -f /pmem0p1/kevin/pools/map -T ${bench} > ${dir_name}/${numrows}rows_${bench}_log${iter} 
        done
    done
done

for numrows in ${numrows_list[@]}; do
    for bench in hashmap_atomic hashmap_tx ctree btree rbtree rtree; do
    #for bench in rtree; do
        for iter in $(seq 0 $iters_minus_1); do
            # grab the elapsed time from benchmark output log: first number on the last line
            time=$(cat ${dir_name}/${numrows}rows_${bench}_log${iter} | tail -n 1 | cut -d";" -f1)
            # check if there are mismatches
            if cat ${dir_name}/${numrows}rows_${bench}_log${iter} | grep -q "Checksum mismatch" ; then
                echo "found mismatch in file ${dir_name}/${numrows}rows_${bench}_log${iter}"
                echo $bench,ERR_MISMATCH >> ${dir_name}/${csv_name}.csv
            else
                # no mismatches     
                echo $numrows,$bench,$time >> ${dir_name}/${csv_name}.csv
            fi  
    
        done
    done
done

