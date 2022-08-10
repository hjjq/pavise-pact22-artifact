#!/bin/bash

# Make pushd and popd silent
pushd () {
    command pushd "$@" > /dev/null
}
popd () {
    command popd "$@" > /dev/null
}
export pushd popd

### Preproduce Pavise ignore list results
echo "======================================="
echo "Preproducing Pavise ignore list results"
# Edit PMDK user.mk to use Pavise conservative tracking pass
printf "CC=clang
CXX=clang++
EXTRA_CFLAGS = -g -Wno-error -fexperimental-new-pass-manager -pavise=pavisenoload" > $PAVISE_ROOT/pmdk-1.10/user.mk
# Recompile PMDK with new pass
echo "Recompiling PMDK with new pass..."
pushd $PAVISE_ROOT/pmdk-1.10
make clean -j &> /dev/null
make -j &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! PMDK build failed." 
    exit 1
fi
echo "PMDK compilation finished successfully."
popd 
## Run microbenchmarks
echo "Running microbenchmarks with ignore list"
pushd $PAVISE_ROOT/pmdk-1.10/src/benchmarks
source pact22_repro_microbench_ignorelist.sh
popd


### Preproduce Pavise conservative results
echo "============================================="
echo "Preproducing Pavise conservative list results"
# Edit PMDK user.mk to use Pavise conservative tracking pass
printf "CC=clang
CXX=clang++
EXTRA_CFLAGS = -g -Wno-error -fexperimental-new-pass-manager -pavise=pavisenoload_conservative" > $PAVISE_ROOT/pmdk-1.10/user.mk
# Recompile PMDK with new pass
echo "Recompiling PMDK with new pass"
pushd $PAVISE_ROOT/pmdk-1.10
make clean -j &> /dev/null
make -j &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! PMDK build failed." 
    exit 1
fi
popd 
## Run microbenchmarks
echo "Running microbenchmarks with conservative tracking"
pushd $PAVISE_ROOT/pmdk-1.10/src/benchmarks
source pact22_repro_microbench_conservative.sh
popd

