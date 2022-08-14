rm -rf tx-build 
mkdir tx-build
cp -f CMakeLists-tx.txt CMakeLists.txt
cd tx-build
cmake ../
make -j12
cd ../
rm -rf notx-build 
mkdir notx-build
cp -f CMakeLists-notx.txt CMakeLists.txt
cd notx-build
cmake ../
make -j12
cd ../
