cmake_minimum_required(VERSION 3.3)
project(pmap_test CXX)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
set(CMAKE_CXX_STANDARD 11)
set(PMDK "/home/swapnilh/pmdk-stuff/pmdk/src/")

add_executable(pmap_test pmap_test.cpp)
target_include_directories(pmap_test PUBLIC ${PMDK}/include/ ${PMDK} ${PMDK}/examples/libpmemobj/hashmap/ . ..)
target_link_libraries(pmap_test  ${CMAKE_THREAD_LIBS_INIT} -lpthread -L${PMDK}/nondebug/ -lpmemobj -lpmem -L${PMDK}/examples/libpmemobj/hashmap -lhashmap_tx)
