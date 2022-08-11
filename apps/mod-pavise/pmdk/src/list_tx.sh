source source.sh
rm -rf /mnt/ext4-pmem22/list.pool 
echo "TX list"
./tools/pmempool/pmempool create obj /mnt/ext4-pmem22/list.pool --layout list --size 1GB
taskset 0x100 ./examples/libpmemobj/linkedlist/fifo /mnt/ext4-pmem22/list.pool 1000000 insert h
#taskset 0x100 ./examples/libpmemobj/linkedlist/fifo /mnt/ext4-pmem22/list.pool 1 print 
sleep 2
taskset 0x100 ./examples/libpmemobj/linkedlist/fifo /mnt/ext4-pmem22/list.pool 1000000 remove

echo "NOTX list"
rm -rf /mnt/ext4-pmem22/list.pool 
./tools/pmempool/pmempool create obj /mnt/ext4-pmem22/list.pool --layout list --size 1GB
taskset 0x100 ./examples/libpmemobj/linkedlist/fifo_notx /mnt/ext4-pmem22/list.pool 1000000 insert h
#taskset 0x100 ./examples/libpmemobj/linkedlist/fifo_notx /mnt/ext4-pmem22/list.pool 1 print 
sleep 2
taskset 0x100 ./examples/libpmemobj/linkedlist/fifo_notx /mnt/ext4-pmem22/list.pool 1000000 remove
