# pavise
Priority data protection for Persistent Memory programs and libraries

## Setting env
```
export PIN_ROOT=${pavise root}/pin-3.10
```
## Build Pavise
```
make
```
## Build PMDK
```
cd pmdk
make
```
## Build Pintool
```
cd src/pintool
make PIN_ROOT=$PIN_ROOT obj-intel64/pintool.so
```
## Run program with pintool
```
cd pmdk/src/examples/libpmemobj/map
$PIN_ROOT/pin -t ${pavise root}/src/pintool/obj-intel64/pintool.so -o ${output file path} -- ./data_store hashmap_atomic ${PM file path} 5
```
