cmake_minimum_required(VERSION 3.3)
project(pmap_test CXX)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
set(CMAKE_CXX_STANDARD 11)
set(PMEM_LIBCXX "/home/swapnilh/pmdk-stuff/libcxx/build/")

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
	pkg_check_modules(LIBPMEMOBJ++ REQUIRED libpmemobj++)
else()
	find_package(LIBPMEMOBJ++ REQUIRED)
endif()

link_directories(${LIBPMEMOBJ++_LIBRARY_DIRS})

add_executable(pmap_test pmap_test.cpp)
target_include_directories(pmap_test PUBLIC ${LIBPMEMOBJ++_INCLUDE_DIRS} "${PMEM_LIBCXX}/include" . ..)
target_link_libraries(pmap_test ${LIBPMEMOBJ++_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -lpthread -L"${PMEM_LIBCXX}/lib/" -lc++)
