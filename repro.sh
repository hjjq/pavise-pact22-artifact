#!/bin/bash

# Make pushd and popd silent
pushd () {
    command pushd "$@" > /dev/null
}
popd () {
    command popd "$@" > /dev/null
}
export pushd popd

### Compile Pavise shared library
echo "======================================="
echo "Compiling Pavise shared library."
pushd $PAVISE_ROOT
make a_runtime_eval &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! Pavise build failed." 
    exit 1
fi
echo "Pavise compilation finished successfully."
popd 

### Preproduce Pavise ignore list results
echo "======================================="
echo "Preproducing Pavise ignore list results"
# Edit PMDK user.mk to use Pavise conservative tracking pass
printf "CC=clang 
CXX=clang++
EXTRA_CFLAGS = -g -Wno-error -fexperimental-new-pass-manager -pavise=pavisenoload" > $PAVISE_ROOT/pmdk-1.10/user.mk
# Recompile PMDK with new pass
echo "Recompiling PMDK with new pass... (~3 min)"
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
# Modify LD_LIBRARY_PATH to use PMDK without Pavise
export LD_LIBRARY_PATH=$PAVISE_ROOT/pmdk-1.10/src/nondebug:$PAVISE_ROOT/build/lib:$PAVISE_ROOT/isa-l/lib:$PAVISE_ROOT/pmdk-1.10-no_pavise/src/examples/libpmemobj/hashmap:/usr/local/lib64:/usr/local/lib:/usr/lib/x86_64-linux-gnu
# Recompile PMDK with new pass
echo "Recompiling PMDK with new pass... (~3 min)"
pushd $PAVISE_ROOT/pmdk-1.10
make clean -j &> /dev/null
make -j &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! PMDK build failed." 
    exit 1
fi
echo "Pavise compilation finished successfully."
popd 
## Run microbenchmarks
echo "Running microbenchmarks with conservative tracking"
pushd $PAVISE_ROOT/pmdk-1.10/src/benchmarks
source pact22_repro_microbench_conservative.sh
popd


### Preproduce PMDK without Pavise results
echo "============================================="
echo "Preproducing Pavise conservative list results"
# Edit PMDK user.mk to use Pavise conservative tracking pass
printf "CC=clang
CXX=clang++
EXTRA_CFLAGS = -g -Wno-error -fexperimental-new-pass-manager" > $PAVISE_ROOT/pmdk-1.10-no_pavise/user.mk
# Modify LD_LIBRARY_PATH to use PMDK without Pavise
export LD_LIBRARY_PATH=$PAVISE_ROOT/pmdk-1.10-no_pavise/src/nondebug:$PAVISE_ROOT/build/lib:$PAVISE_ROOT/isa-l/lib:$PAVISE_ROOT/pmdk-1.10-no_pavise/src/examples/libpmemobj/hashmap:/usr/local/lib64:/usr/local/lib:/usr/lib/x86_64-linux-gnu
# Recompile PMDK with new pass
echo "Recompiling PMDK with new pass... (~3 min)" 
pushd $PAVISE_ROOT/pmdk-1.10-no_pavise
make clean -j &> /dev/null
make -j &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! PMDK build failed." 
    exit 1
fi
echo "Pavise compilation finished successfully."
popd 
## Run microbenchmarks
echo "Running microbenchmarks with conservative tracking"
pushd $PAVISE_ROOT/pmdk-1.10-no_pavise/src/benchmarks
source pact22_repro_microbench_pmdk.sh
popd

echo "\n"
echo "Finished reproducing microbenchmarks. Results are stored in $PAVISE_ROOT/results"
