export PMEM_IS_PMEM_FORCE=1
export PMEM_MMAP_HINT=0x10000000000
rm -rf temp/; ./build/vacation temp -r100000 -t200000 -n1 -q80 -u99 
