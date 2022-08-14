cd src/
rm -rf nondebug debug tx-nondebug notx-nondebug
echo "********** Compiling PMDK w/ logging **********"
cp -f libpmemobj/Makefile-tx libpmemobj/Makefile
make clean; make -j12 libpmem libpmemobj examples
mv nondebug/ tx-nondebug
cp examples/libpmemobj/hashmap/*.so tx-nondebug/
cp examples/libpmemobj/map/*.so tx-nondebug/

echo "********** Compiling PMDK withOUT logging **********"
cp -f libpmemobj/Makefile-notx libpmemobj/Makefile
make clean; make -j12 libpmem libpmemobj examples
mv nondebug/ notx-nondebug
cp examples/libpmemobj/hashmap/*.so notx-nondebug/
cp examples/libpmemobj/map/*.so notx-nondebug/
