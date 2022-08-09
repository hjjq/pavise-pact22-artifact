#!/bin/bash

# create temp dir to store logs
dir_name="numrows_large100000"
csv_name=$dir_name

#row_size_list=(20 30 40 50 60 70 80 90 100)
numrows_list=(500 1000 2000 5000 10000 50000 100000)
bench_list=(hashmap_atomic hashmap_tx ctree btree rbtree rtree)

mkdir -p $dir_name
echo "num rows,benchmark,elapsed time in seconds" > ${dir_name}/${csv_name}.csv
for numrows in ${numrows_list[@]}; do
    # modify PARITY_NUM_ROWS in source header
    sed -i "s/#define PARITY_NUM_ROWS.*/#define PARITY_NUM_ROWS ${numrows}/" $PAVISE_ROOT/include/pavise.hpp
    # remake pavise and return to benchmark directory
    cd $PAVISE_ROOT
    make a
    cd $PAVISE_ROOT/pmdk-1.10/src/benchmarks 
    for bench in ${bench_list[@]}; do
        # run each bench multiple times
        for iter in {0..2}; do
            echo Running num rows $numrows for $bench the $iter th time
            rpf
            ./pmembench map_insert -n 1000000 -d 256 -f /pmem0p1/nvm-admin/pmdk/map -T ${bench} > ${dir_name}/numrows${numrows}_${bench}_log${iter} 
        done
    done
done

# collect results by grepping output logs
for numrows in ${numrows_list[@]}; do
    for bench in ${bench_list[@]}; do
        for iter in {0..2}; do
            # grab the elapsed time from benchmark output log: first number on the last line
            time=$(cat ${dir_name}/numrows${numrows}_${bench}_log${iter} | tail -n 1 | cut -d";" -f1)
            echo $numrows,$bench,$time >> ${dir_name}/${csv_name}.csv
        done
    done
done
