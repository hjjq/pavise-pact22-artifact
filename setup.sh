#!/bin/bash


export PAVISE_ROOT=$PWD
export PMEM_MMAP_HINT=0x10000000000
export PAVISE_LLVM_ROOT=$PAVISE_ROOT/llvm
export LD_LIBRARY_PATH=$PAVISE_ROOT/pmdk-1.10/src/nondebug:$PAVISE_ROOT/build/lib:$PAVISE_ROOT/isa-l/lib:$PAVISE_ROOT/pmdk-1.10/src/examples/libpmemobj/hashmap:/usr/local/lib64:/usr/local/lib:/usr/lib/x86_64-linux-gnu
export PATH=/ssd1/llvm-project/build/bin:$PATH


#echo "PAVISE_ROOT = $PAVISE_ROOT"
#echo "PMEM_MMAP_HINT = $PMEM_MMAP_HINT"
#echo "PAVISE_LLVM_ROOT = $PAVISE_LLVM_ROOT"
#echo "LD_LIBRARY_PATH = $LD_LIBRARY_PATH"
#echo "PATH = $PATH"


# Edit file paths in whitelist
sed "s@PAVISE_ROOT@$PAVISE_ROOT@g" llvm/llvmlog_template > llvm/llvmlog
