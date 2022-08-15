cmake_minimum_required(VERSION 3.3)
project(vacation CXX)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -DLOGGING_ON")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O2 -mclflushopt")
set(PMEM_LIBCXX ${CMAKE_SOURCE_DIR}../libcxx/build/)
set(PMDK ${CMAKE_SOURCE_DIR}/../pmdk/src/)
set(VALGRIND ${CMAKE_SOURCE_DIR}/..//pmem-valgrind/)

link_directories(../libpmemobj-cpp/nondebug/)

add_executable(vacation vacation.cpp manager.cpp customer.cpp reservation.cpp)
target_include_directories(vacation PUBLIC ${PMDK}/examples/libpmemobj/hashmap ${PMDK}/include ${VALGRIND}/include/ ${VALGRIND}/lackey/ ../libpmemobj-cpp/include "${PMEM_LIBCXX}/include"  . ..)
target_link_libraries(vacation ${LIBPMEMOBJ++_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -lpthread -L"${PMDK}/tx-nondebug" -lpmem  -lpmemobj -L"${PMDK}/tx-nondebug" -lhashmap_tx)


add_executable(vacation_notx vacation.cpp manager.cpp customer.cpp reservation.cpp)
target_include_directories(vacation_notx PUBLIC ${PMDK}/examples/libpmemobj/hashmap ${PMDK}/include ${VALGRIND}/include/ ${VALGRIND}/lackey/ ../libpmemobj-cpp/include "${PMEM_LIBCXX}/include"  . ..)
target_link_libraries(vacation_notx ${LIBPMEMOBJ++_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -lpthread -L"${PMDK}/tx-nondebug" -lpmem  -lpmemobj -L"${PMDK}/notx-nondebug" -lhashmap_notx)
target_compile_options(vacation_notx PRIVATE  -DNO_TX)
