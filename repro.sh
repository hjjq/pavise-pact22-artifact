#!/bin/bash

# Make pushd and popd silent
pushd () {
    command pushd "$@" > /dev/null
}
popd () {
    command popd "$@" > /dev/null
}
export pushd popd


source setup.sh
echo "PAVISE_ROOT = $PAVISE_ROOT"
### Compile ISA-L
echo "======================================="
echo "Compiling ISA-L... (~ mins)"
pushd $PAVISE_ROOT/isa-l
./autogen.sh &> /dev/null
./configure --prefix=$PAVISE_ROOT/isa-l --libdir=$PAVISE_ROOT/isa-l/lib &> /dev/null
make &> /dev/null
make install &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! ISA-L build failed." 
    exit 1
fi
echo "ISA-L compilation finished successfully."
popd 

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
# Edit PMDK user.mk to use Pavise ignore lsit pass
printf "CC=clang 
CXX=clang++
EXTRA_CFLAGS = -g -Wno-error -fexperimental-new-pass-manager -pavise=pavisenoload" > $PAVISE_ROOT/pmdk-1.10/user.mk
### Recompile PMDK with new pass
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
### Run microbenchmarks
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
# Modify LD_LIBRARY_PATH 
export LD_LIBRARY_PATH=$PAVISE_ROOT/pmdk-1.10/src/nondebug:$PAVISE_ROOT/build/lib:$PAVISE_ROOT/isa-l/lib:$PAVISE_ROOT/pmdk-1.10/src/examples/libpmemobj/hashmap:/usr/local/lib64:/usr/local/lib:/usr/lib/x86_64-linux-gnu
### Recompile PMDK with new pass
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
### Run microbenchmarks
echo "Running microbenchmarks with conservative tracking"
pushd $PAVISE_ROOT/pmdk-1.10/src/benchmarks
source pact22_repro_microbench_conservative.sh
popd


### Preproduce PMDK without Pavise results
echo "============================================="
echo "Preproducing PMDK without Pavise results"
# Edit PMDK user.mk 
printf "CC=clang
CXX=clang++
EXTRA_CFLAGS = -g -Wno-error -fexperimental-new-pass-manager" > $PAVISE_ROOT/pmdk-1.10-no_pavise/user.mk
# Modify LD_LIBRARY_PATH to use PMDK without Pavise
export LD_LIBRARY_PATH=$PAVISE_ROOT/pmdk-1.10-no_pavise/src/nondebug:$PAVISE_ROOT/build/lib:$PAVISE_ROOT/isa-l/lib:$PAVISE_ROOT/pmdk-1.10-no_pavise/src/examples/libpmemobj/hashmap:/usr/local/lib64:/usr/local/lib:/usr/lib/x86_64-linux-gnu
### Recompile PMDK with new pass
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
### Run microbenchmarks
echo "Running microbenchmarks without Pavise"
pushd $PAVISE_ROOT/pmdk-1.10-no_pavise/src/benchmarks
source pact22_repro_microbench_pmdk.sh
popd

echo "\n"
echo "Finished reproducing microbenchmarks. Results are stored in $PAVISE_ROOT/results"
echo "Reproducing real applications in 3 seconds..."
sleep 3

##################################################
# Setup for real applications
##################################################
### Build vacation PMDK (WHISPER)
echo "======================================="
echo "Prepare environment for real applications..."
echo "Compiling WHISPER PMDK..."
pushd $PAVISE_ROOT/apps/mod-pavise/pmdk
bash compile.sh &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! PMDK build failed." 
    exit 1
fi
echo "PMDK compilation finished successfully."
popd
### Build pmem-valgrind
echo "Compiling pmem-valgrind..."
pushd $PAVISE_ROOT/apps/mod-pavise/pmem-valgrind
./autogen.sh &> /dev/null
./configure &> /dev/null
make -j &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! PMDK build failed." 
    exit 1
fi
echo "pmem-valgrind compilation finished successfully."
popd

###################################################
# Vacation ignore list
###################################################
### Preproduce vacation  + Pavise ignore list
echo "======================================="
echo "Preproducing vacation + Pavise ignore list"
# Edit PMDK user.mk to use Pavise ignorelist pass
printf "CC=clang 
CXX=clang++
EXTRA_CFLAGS = -g -Wno-error -fexperimental-new-pass-manager -pavise=pavisenoload" > $PAVISE_ROOT/pmdk-1.10/user.mk
### Recompile PMDK with ignore list pass
echo "Recompiling PMDK with ignore list pass... (~3 min)"
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
### Build vacation
echo "Building vacation..."
pushd $PAVISE_ROOT/apps/mod-pavise/vacation-pmdk
# Use Pavise ignore list pass
sed -i 's@set(CMAKE_CXX_FLAGS.*@set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O2 -mclwb -Wno-error=unused-command-line-argument -fexperimental-new-pass-manager -pavise=pavisenoload")@' ./CMakeLists.txt
bash compile.sh  &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! vacaction build failed." 
    exit 1
fi
echo "vacation compilation finished successfully."
popd
### Run vacation
echo "Running vacation with ignore list"
pushd $PAVISE_ROOT/apps/mod-pavise/vacation-pmdk/build
rm -rf /pmem0p1/kevin/pools/*
./vacation /pmem0p1/kevin/pools/vacation -r100000 -t200000 -n1 -q55 -u99 &> $PAVISE_ROOT/results/vacation_ignorelist
echo "Finished running vacation."
popd

##########
# Reproduce memcached-W  + Pavise ignore list
##########
echo "======================================="
echo "Preproducing memcached-W + Pavise ignore list"
# Modify LD_LIBRARY_PATH 
export LD_LIBRARY_PATH=$PAVISE_ROOT/pmdk-1.10/src/nondebug:$PAVISE_ROOT/build/lib:$PAVISE_ROOT/isa-l/lib:$PAVISE_ROOT/pmdk-1.10/src/examples/libpmemobj/hashmap:/usr/local/lib64:/usr/local/lib:/usr/lib/x86_64-linux-gnu

########## Build memcached-W
echo "Building memcached-W..."
pushd $PAVISE_ROOT/apps/mod-pavise/memcached-pmdk
sed -i 's@set(CMAKE_CXX_FLAGS.*@set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_CONFIG_H -O2 -ggdb -mclflushopt -lpthread -DTX -fexperimental-new-pass-manager -pavise=pavisenoload"))@' ./CMakeLists.txt
sed -i 's@set(CMAKE_C_FLAGS.*@set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_CONFIG_H -O2 -ggdb -mclflushopt -lpthread -DTX -fexperimental-new-pass-manager -pavise=pavisenoload"))@' ./CMakeLists.txt
bash compile.sh  &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! memcached-W build failed." 
    exit 1
fi
echo "memcached-W compilation finished successfully."
popd
########## Run memcached-W
echo "Starting memcached-W server..."
pushd $PAVISE_ROOT/apps/mod-pavise/memcached-pmdk/tx-build
rm -rf /pmem0p1/kevin/pools/*
./memcached /pmem0p1/kevin/pools/memcached-pmdk -u root -p 11211 -l 127.0.0.1 -t 1 &
PID_memcached=$!
sleep 5 # make sure the server is fully started
popd
pushd $PAVISE_ROOT/apps/mod-pavise/libmemcached-1.0.18/clients
echo "Starting memcached-W client..."
./memaslap -s 127.0.0.1:11211 -c 4 -x 100000 -T 4 -X 512 -F ./run.cnf -d 1 &> $PAVISE_ROOT/results/memcached-W_ignorelist
kill $PID_memcached
sleep 3 # make sure the server is fully terminated
echo "Finished running memcached-W."
popd

###################################################
# memcached-L ignore list
###################################################
### Preproduce memcached-L  + Pavise ignore list
echo "======================================="
echo "Preproducing memcached-L + Pavise ignore list"
### Build memcached-L
echo "Building memcached-L..."
pushd $PAVISE_ROOT/apps/memcached-pavise
# Use Pavise ignore list pass
sed -i 's@EXTRA_CFLAGS = -g.*@EXTRA_CFLAGS = -g -I${PAVISE_ROOT}/include -L${PAVISE_ROOT}/build/lib -Wno-error=unused-command-line-argument -fexperimental-new-pass-manager -pavise=pavisenoload@' ./user.mk
make -j  &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! memcached-L build failed." 
    exit 1
fi
echo "memcached-L compilation finished successfully."
popd
### Run memcached-L
echo "Running memcached-L with ignore list"
pushd $PAVISE_ROOT/apps/memcached-pavise
rm -rf /pmem0p1/kevin/pools/*
./memcached -u root -m 0 -t 1 -o pslab_file=/pmem0p1/kevin/pools/memcached-l,pslab_force &
PID_memcached_L=$!
sleep 5 # make sure the server is fully started
echo "Starting memcached-L client..."
memtier_benchmark -p 11211 -P memcache_binary -n 100000 --ratio=1:0 -d 256 -t 1 &> $PAVISE_ROOT/results/memcached-L_ignorelist
kill $PID_memcached_L
sleep 3 # make sure the server is fully terminated
echo "Finished running memcached-L."
popd


###################################################
# Redis ignore list
###################################################
### Preproduce redis  + Pavise ignore list
echo "======================================="
echo "Preproducing redis + Pavise ignore list"
## Build redis
echo "Building redis..."
pushd $PAVISE_ROOT/apps/redis
sed -i 's@PAVISE_CFLAGS=.*@PAVISE_CFLAGS=-fexperimental-new-pass-manager -pavise=pavisenoload@' ./src/Makefile
make -j USE_PMDK=yes STD=-std=gnu99  &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! redis build failed." 
    exit 1
fi
echo "redis compilation finished successfully."
popd
### Run redis
echo "Running redis with ignore list"
pushd $PAVISE_ROOT/apps/redis
rm -rf /pmem0p1/kevin/pools/*
# First shutdown any existing redis servers
./src/redis-cli shutdown  &> /dev/null 
./src/redis-server redis.conf &
PID_redis=$!
sleep 5 # make sure the server is fully started
echo "Starting redis client..."
memtier_benchmark -n 100000 --ratio=1:0 -d 256 &> $PAVISE_ROOT/results/redis_ignorelist
kill $PID_redis
sleep 3 # make sure the server is fully terminated
echo "Finished running redis."
popd

###################################################
# Preproduce vacation  + Pavise conservative
###################################################
echo "======================================="
echo "Preproducing vacation + Pavise conservative tracking"
# Edit PMDK user.mk to use Pavise conservative
printf "CC=clang 
CXX=clang++
EXTRA_CFLAGS = -g -Wno-error -fexperimental-new-pass-manager -pavise=pavisenoload_conservative" > $PAVISE_ROOT/pmdk-1.10/user.mk
# Modify LD_LIBRARY_PATH 
export LD_LIBRARY_PATH=$PAVISE_ROOT/pmdk-1.10/src/nondebug:$PAVISE_ROOT/build/lib:$PAVISE_ROOT/isa-l/lib:$PAVISE_ROOT/pmdk-1.10/src/examples/libpmemobj/hashmap:/usr/local/lib64:/usr/local/lib:/usr/lib/x86_64-linux-gnu
### Recompile PMDK with conservative pass
echo "Recompiling PMDK with conservative pass... (~3 min)"
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
### Build vacation
echo "Building vacation..."
pushd $PAVISE_ROOT/apps/mod-pavise/vacation-pmdk
sed -i 's@set(CMAKE_CXX_FLAGS.*@set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O2 -mclwb -Wno-error=unused-command-line-argument -fexperimental-new-pass-manager -pavise=pavisenoload_conservative")@' ./CMakeLists.txt
bash compile.sh  &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! vacaction build failed." 
    exit 1
fi
echo "vacation compilation finished successfully."
popd
### Run vacation
echo "Running vacation with conservative"
pushd $PAVISE_ROOT/apps/mod-pavise/vacation-pmdk/build
rm -rf /pmem0p1/kevin/pools/*
./vacation /pmem0p1/kevin/pools/vacation -r100000 -t200000 -n1 -q55 -u99 &> $PAVISE_ROOT/results/vacation_conservative
echo "Finished running vacation."
popd

##########
# Reproduce memcached-W  + Pavise conservative
##########
echo "======================================="
echo "Preproducing memcached-W + Pavise conservative tracking"
# Edit PMDK user.mk to use Pavise conservative
printf "CC=clang 
CXX=clang++
EXTRA_CFLAGS = -g -Wno-error -fexperimental-new-pass-manager -pavise=pavisenoload_conservative" > $PAVISE_ROOT/pmdk-1.10/user.mk
# Modify LD_LIBRARY_PATH 
export LD_LIBRARY_PATH=$PAVISE_ROOT/pmdk-1.10/src/nondebug:$PAVISE_ROOT/build/lib:$PAVISE_ROOT/isa-l/lib:$PAVISE_ROOT/pmdk-1.10/src/examples/libpmemobj/hashmap:/usr/local/lib64:/usr/local/lib:/usr/lib/x86_64-linux-gnu

########## Build memcached-W
echo "Building memcached-W..."
pushd $PAVISE_ROOT/apps/mod-pavise/memcached-pmdk
sed -i 's@set(CMAKE_CXX_FLAGS.*@set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_CONFIG_H -O2 -ggdb -mclflushopt -lpthread -DTX -fexperimental-new-pass-manager -pavise=pavisenoload_conservative"))@' ./CMakeLists.txt
sed -i 's@set(CMAKE_C_FLAGS.*@set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_CONFIG_H -O2 -ggdb -mclflushopt -lpthread -DTX -fexperimental-new-pass-manager -pavise=pavisenoload_conservative"))@' ./CMakeLists.txt
bash compile.sh  &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! memcached-W build failed." 
    exit 1
fi
echo "memcached-W compilation finished successfully."
popd
########## Run memcached-W
echo "Starting memcached-W server..."
pushd $PAVISE_ROOT/apps/mod-pavise/memcached-pmdk/tx-build
rm -rf /pmem0p1/kevin/pools/*
./memcached /pmem0p1/kevin/pools/memcached-pmdk -u root -p 11211 -l 127.0.0.1 -t 1 &
sleep 5 # make sure the server is fully started
PID_memcached=$!
popd 
pushd $PAVISE_ROOT/apps/mod-pavise/libmemcached-1.0.18/clients
echo "Starting memcached-W client..."
./memaslap -s 127.0.0.1:11211 -c 4 -x 100000 -T 4 -X 512 -F ./run.cnf -d 1 &> $PAVISE_ROOT/results/memcached-W_conservative
kill $PID_memcached
sleep 3 # make sure the server is fully terminated
echo "Finished running memcached-W."
popd


##################################################
# Preproduce memcached-L  + Pavise conservative
##################################################
echo "======================================="
echo "Preproducing memcached-L + Pavise conservative tracking"
# Modify LD_LIBRARY_PATH 
export LD_LIBRARY_PATH=$PAVISE_ROOT/pmdk-1.10/src/nondebug:$PAVISE_ROOT/build/lib:$PAVISE_ROOT/isa-l/lib:$PAVISE_ROOT/pmdk-1.10/src/examples/libpmemobj/hashmap:/usr/local/lib64:/usr/local/lib:/usr/lib/x86_64-linux-gnu
### Build memcached-L
echo "Building memcached-L..."
pushd $PAVISE_ROOT/apps/memcached-pavise
# Use Pavise conservative tracking pass
sed -i 's@EXTRA_CFLAGS = -g.*@EXTRA_CFLAGS = -g -I${PAVISE_ROOT}/include -L${PAVISE_ROOT}/build/lib -Wno-error=unused-command-line-argument -fexperimental-new-pass-manager -pavise=pavisenoload_conservative@' ./user.mk
make -j  &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! memcached-L build failed." 
    exit 1
fi
echo "memcached-L compilation finished successfully."
popd
### Run memcached-L
echo "Running memcached-L with conservative"
pushd $PAVISE_ROOT/apps/memcached-pavise
rm -rf /pmem0p1/kevin/pools/*
./memcached -u root -m 0 -t 1 -o pslab_file=/pmem0p1/kevin/pools/memcached-l,pslab_force &
PID_memcached_L=$!
sleep 5 # make sure the server is fully started
echo "Starting memcached-L client..."
memtier_benchmark -p 11211 -P memcache_binary -n 100000 --ratio=1:0 -d 256 -t 1 &> $PAVISE_ROOT/results/memcached-L_conservative
kill $PID_memcached_L
sleep 3 # make sure the server is fully terminated
echo "Finished running memcached-L."
popd


###################################################
# Preproduce redis  + Pavise conservative
###################################################
echo "======================================="
echo "Preproducing redis + Pavise conservative tracking"
# Modify LD_LIBRARY_PATH 
export LD_LIBRARY_PATH=$PAVISE_ROOT/pmdk-1.10/src/nondebug:$PAVISE_ROOT/build/lib:$PAVISE_ROOT/isa-l/lib:$PAVISE_ROOT/pmdk-1.10/src/examples/libpmemobj/hashmap:/usr/local/lib64:/usr/local/lib:/usr/lib/x86_64-linux-gnu
### Build redis
echo "Building redis..."
pushd $PAVISE_ROOT/apps/redis
sed -i 's@PAVISE_CFLAGS=.*@PAVISE_CFLAGS=-fexperimental-new-pass-manager -pavise=pavisenoload_conservative@' ./src/Makefile
make -j USE_PMDK=yes STD=-std=gnu99  &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! redis build failed." 
    exit 1
fi
echo "redis compilation finished successfully."
popd
### Run redis
echo "Running redis with ignore list"
pushd $PAVISE_ROOT/apps/redis
rm -rf /pmem0p1/kevin/pools/*
# First shutdown any existing redis servers
./src/redis-cli shutdown &> /dev/null
./src/redis-server redis.conf &
PID_redis=$!
sleep 5 # make sure the server is fully started
echo "Starting redis client..."
memtier_benchmark -n 100000 --ratio=1:0 -d 256 &> $PAVISE_ROOT/results/redis_conservative
kill $PID_redis
sleep 3 # make sure the server is fully terminated
echo "Finished running redis."
popd


##################################################
# Setup for no pavise runs
##################################################
### Build vacation PMDK (WHISPER)
echo "======================================="
echo "Prepare environment for real applications..."
echo "Compiling WHISPER PMDK..."
pushd $PAVISE_ROOT/apps-no_pavise/mod-single-repo/pmdk
bash compile.sh &> /dev/null
if [ $? -ne 0 ];
then
    echo "ERROR! PMDK build failed."
    exit 1
fi
echo "PMDK compilation finished successfully."
popd

##################################################
# Preproduce memcached-L  + no Pavise 
##################################################
# Edit PMDK user.mk
printf "CC=clang
CXX=clang++
EXTRA_CFLAGS = -g -Wno-error -fexperimental-new-pass-manager" > $PAVISE_ROOT/pmdk-1.10-no_pavise/user.mk
# Modify LD_LIBRARY_PATH to use PMDK without Pavise
export LD_LIBRARY_PATH=$PAVISE_ROOT/pmdk-1.10-no_pavise/src/nondebug:$PAVISE_ROOT/build/lib:$PAVISE_ROOT/isa-l/lib:$PAVISE_ROOT/pmdk-1.10-no_pavise/src/examples/libpmemobj/hashmap:/usr/local/lib64:/usr/local/lib:/usr/lib/x86_64-linux-gnu
### Recompile PMDK with new pass
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


#### Preproduce memcached-L  + no Pavise
echo "======================================="
echo "Preproducing memcached-L + no Pavise"
# Use PMDK with no pavise
export LD_LIBRARY_PATH=$PAVISE_ROOT/pmdk-1.10-no_pavise/src/nondebug:$PAVISE_ROOT/build/lib:$PAVISE_ROOT/isa-l/lib:$PAVISE_ROOT/pmdk-1.10-no_pavise/src/examples/libpmemobj/hashmap:/usr/local/lib64:/usr/local/lib:/usr/lib/x86_64-linux-gnu
### Build memcached-L
echo "Building memcached-L..."
pushd $PAVISE_ROOT/apps-no_pavise/memcached-pmem
make -j  &> /dev/null
if [ $? -ne 0 ];
then
    echo "ERROR! memcached-L build failed."
    exit 1
fi
echo "memcached-L compilation finished successfully."
popd
### Run memcached-L
echo "Running memcached-L with no pavise"
pushd $PAVISE_ROOT/apps-no_pavise/memcached-pmem
rm -rf /pmem0p1/kevin/pools/*
./memcached -u root -m 0 -t 1 -o pslab_file=/pmem0p1/kevin/pools/memcached-l,pslab_force &
PID_memcached_L=$!
sleep 5 # make sure the server is fully started
echo "Starting memcached-L client..."
memtier_benchmark -p 11211 -P memcache_binary -n 100000 --ratio=1:0 -d 256 -t 1 &> $PAVISE_ROOT/results/memcached-L_no_pavise
kill $PID_memcached_L
sleep 3 # make sure the server is fully terminated
echo "Finished running memcached-L."
popd

### Preproduce redis  + no Pavise
echo "======================================="
echo "Preproducing redis + no Pavise"
## Build redis
echo "Building redis..."
pushd $PAVISE_ROOT/apps-no_pavise/redis
make -j USE_PMDK=yes STD=-std=gnu99  &> /dev/null
if [ $? -ne 0 ];
then
    echo "ERROR! redis build failed."
    exit 1
fi
echo "redis compilation finished successfully."
popd
### Run redis
echo "Running redis with no pavise"
pushd $PAVISE_ROOT/apps-no_pavise/redis
rm -rf /pmem0p1/kevin/pools/*
# First shutdown any existing redis servers
./src/redis-cli shutdown  &> /dev/null
./src/redis-server redis.conf &
PID_redis=$!
sleep 5 # make sure the server is fully started
echo "Starting redis client..."
memtier_benchmark -n 100000 --ratio=1:0 -d 256 &> $PAVISE_ROOT/results/redis_no_pavise
kill $PID_redis
sleep 3 # make sure the server is fully terminated
echo "Finished running redis"


##########
# Reproduce memcached-W  + no Pavise
##########
echo "======================================="
echo "Preproducing memcached-W + no Pavise"

########## Build memcached-W
echo "Building memcached-W..."
pushd $PAVISE_ROOT/apps-no_pavise/mod-single-repo/memcached-pmdk
bash compile.sh  &> /dev/null
if [ $? -ne 0 ]; 
then 
    echo "ERROR! memcached-W build failed." 
    exit 1
fi
echo "memcached-W compilation finished successfully."
popd
########## Run memcached-W
echo "Starting memcached-W server..."
pushd $PAVISE_ROOT/apps-no_pavise/mod-single-repo/memcached-pmdk/tx-build
rm -rf /pmem0p1/kevin/pools/*
./memcached /pmem0p1/kevin/pools/memcached-pmdk -u root -p 11211 -l 127.0.0.1 -t 1 &
PID_memcached=$!
sleep 5 # make sure the server is fully started
popd
pushd $PAVISE_ROOT/apps-no_pavise/mod-single-repo/libmemcached-1.0.18/clients
echo "Starting memcached-W client..."
./memaslap -s 127.0.0.1:11211 -c 4 -x 100000 -T 4 -X 512 -F ./run.cnf -d 1 &> $PAVISE_ROOT/results/memcached-W_no_pavise
kill $PID_memcached
sleep 3 # make sure the server is fully terminated
echo "Finished running memcached-W."
popd
