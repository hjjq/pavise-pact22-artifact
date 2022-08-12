rm -rf tx-build 
mkdir tx-build
cp -f CMakeLists-tx.txt CMakeLists.txt
cd tx-build
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ../
#make -j12
make -j
cd ../
#rm -rf notx-build 
#mkdir notx-build
#cp -f CMakeLists-notx.txt CMakeLists.txt
#cd notx-build
#cmake ../
#make -j12
#cd ../
