#
# Copyright 2018-2019, Intel Corporation
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in
#       the documentation and/or other materials provided with the
#       distribution.
#
#     * Neither the name of the copyright holder nor the names of its
#       contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

include(ctest_helpers.cmake)

add_cppstyle(tests-common ${CMAKE_CURRENT_SOURCE_DIR}/common/*.*pp)
add_cppstyle(tests-pmemobj_cow ${CMAKE_CURRENT_SOURCE_DIR}/pmemobj_check_cow/*.*cpp)
add_check_whitespace(tests-pmemobj_cow ${CMAKE_CURRENT_SOURCE_DIR}/pmemobj_check_cow/*.*cpp)
add_check_whitespace(tests-common ${CMAKE_CURRENT_SOURCE_DIR}/common/*.*pp)
add_check_whitespace(tests-cmake ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

if(MSVC_VERSION)
	add_flag(-W4)
else()
	add_flag(-Wall)
endif()
add_flag(-Wpointer-arith)
add_flag(-Wunused-macros)
add_flag(-Wsign-conversion)
add_flag(-Wsign-compare)
add_flag(-Wunreachable-code-return)
add_flag(-Wmissing-variable-declarations)
add_flag(-fno-common)

add_flag(-ggdb DEBUG)
add_flag(-DDEBUG DEBUG)

add_flag("-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2" RELEASE)

if(USE_ASAN)
       add_sanitizer_flag(address)
endif()
if(USE_UBSAN)
       add_sanitizer_flag(undefined)
endif()

if(COVERAGE)
       set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -coverage")
endif()

add_library(test_backtrace STATIC test_backtrace.c)
add_library(valgrind_internal STATIC valgrind_internal.cpp)

function(build_example_queue)
	add_executable(ex-queue ../examples/queue/queue.cpp)
	target_include_directories(ex-queue PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../examples)
	target_link_libraries(ex-queue ${LIBPMEMOBJ_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endfunction()

function(build_example_pman)
	if(CURSES_FOUND AND NOT WIN32)
		add_executable(ex-pman ../examples/pman/pman.cpp)
		target_include_directories(ex-pman PUBLIC ${CURSES_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../examples)
		target_link_libraries(ex-pman ${LIBPMEMOBJ_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${CURSES_LIBRARIES})
	else()
		message(WARNING "ncurses not found - pman test won't be build")
	endif()
endfunction()

find_packages()

if(VALGRIND_FOUND)
	add_flag(-DLIBPMEMOBJ_CPP_VG_MEMCHECK_ENABLED=1)
	add_flag(-DLIBPMEMOBJ_CPP_VG_DRD_ENABLED=1)
	add_flag(-DLIBPMEMOBJ_CPP_VG_HELGRIND_ENABLED=1)

	if(VALGRIND_PMEMCHECK_FOUND)
		add_flag(-DLIBPMEMOBJ_CPP_VG_PMEMCHECK_ENABLED=1)
	endif()
endif()

if(NO_GCC_VARIADIC_TEMPLATE_BUG)
	build_example_queue()
	add_test_generic(ex-queue none)

	build_example_pman()
	add_test_generic(ex-pman none)
else()
	message(WARNING "Skipping examples tests because of gcc variadic template bug")
	skip_test("examples_tests" "SKIPPED_BECAUSE_OF_GCC_VARIADIC_TEMPLATE_BUG")
endif()

if(AGGREGATE_INITIALIZATION_AVAILABLE)
	build_test(aggregate_initialization aggregate_initialization/aggregate_initialization.cpp)
	add_test_generic(aggregate_initialization none)
	add_test_generic(aggregate_initialization pmemcheck)
else()
	message(WARNING "Skipping aggregate initialization test because of no compiler support")
	skip_test("aggregate_initialization" "SKIPPED_BECAUSE_OF_NO_COMPILER_SUPPORT")
endif()

build_test(allocator allocator/allocator.cpp)
add_test_generic(allocator memcheck)
add_test_generic(allocator none)
add_test_generic(allocator pmemcheck)

build_test(detail_common detail_common/detail_common.cpp)
add_test_generic(detail_common none)

build_test(make_persistent make_persistent/make_persistent.cpp)
add_test_generic(make_persistent none)
add_test_generic(make_persistent pmemcheck)

build_test(make_persistent_atomic make_persistent_atomic/make_persistent_atomic.cpp)
add_test_generic(make_persistent_atomic none)
add_test_generic(make_persistent_atomic pmemcheck)

build_test(mutex_posix mutex_posix/mutex_posix.cpp)
add_test_generic(mutex_posix drd)
add_test_generic(mutex_posix helgrind)
add_test_generic(mutex_posix pmemcheck)

build_test(pool pool/pool.cpp)
add_test_generic(pool none 0)
add_test_generic(pool none 1)
add_test_generic(pool none 2)
add_test_generic(pool none 3)

build_test(pool_primitives pool_primitives/pool_primitives.cpp)
add_test_generic(pool_primitives none)
add_test_generic(pool_primitives pmemcheck)

build_test(ptr ptr/ptr.cpp)
add_test_generic(ptr none)
add_test_generic(ptr pmemcheck)

build_test(ptr_arith ptr_arith/ptr_arith.cpp)
add_test_generic(ptr_arith memcheck)
add_test_generic(ptr_arith pmemcheck)
# XXX Bug: incompatibility between asan and custom library
if (NOT USE_ASAN)
	add_test_generic(ptr_arith none)
endif()

build_test(p_ext p_ext/p_ext.cpp)
add_test_generic(p_ext none)
add_test_generic(p_ext pmemcheck)

build_test(shared_mutex_posix shared_mutex_posix/shared_mutex_posix.cpp)
add_test_generic(shared_mutex_posix drd)
add_test_generic(shared_mutex_posix helgrind)
add_test_generic(shared_mutex_posix pmemcheck)

build_test(transaction transaction/transaction.cpp)
add_test_generic(transaction none)
add_test_generic(transaction pmemcheck)

if(WIN32)
	build_test(pool_win pool_win/pool_win.cpp)
	add_test_generic(pool_win none 0)
	add_test_generic(pool_win none 1)
	add_test_generic(pool_win none 2)
	add_test_generic(pool_win none 3)
endif()

if(PMEMVLT_PRESENT)
	build_test(v v/v.cpp)
	add_test_generic(v none)
	add_test_generic(v memcheck)
else()
	message(WARNING "Skipping v test because no pmemvlt support found")
	skip_test("v" "SKIPPED_BECAUSE_OF_MISSING_PMEMVLT")
endif()

if(NO_CHRONO_BUG)
	build_test(cond_var cond_var/cond_var.cpp)
	add_test_generic(cond_var none)

	build_test(cond_var_posix cond_var_posix/cond_var_posix.cpp)
	add_test_generic(cond_var_posix drd)
	add_test_generic(cond_var_posix helgrind)
	add_test_generic(cond_var_posix pmemcheck)

	build_test(mutex mutex/mutex.cpp)
	add_test_generic(mutex none)

	build_test(shared_mutex shared_mutex/shared_mutex.cpp)
	add_test_generic(shared_mutex none)

	build_test(timed_mtx_posix timed_mtx_posix/timed_mtx_posix.cpp)
	add_test_generic(timed_mtx_posix drd)
	add_test_generic(timed_mtx_posix helgrind)
	add_test_generic(timed_mtx_posix pmemcheck)

	build_test(timed_mtx timed_mtx/timed_mtx.cpp)
	add_test_generic(timed_mtx none)
else()
	message(WARNING "Skipping chrono tests because of compiler/stdc++ issues")
	skip_test("chrono_tests" "SKIPPED_BECAUSE_OF_COMPILER_CHRONO_BUG")
endif()

if(NO_CLANG_TEMPLATE_BUG)
	build_test(make_persistent_array make_persistent_array/make_persistent_array.cpp)
	add_test_generic(make_persistent_array none)
	add_test_generic(make_persistent_array pmemcheck)

	build_test(make_persistent_array_atomic make_persistent_array_atomic/make_persistent_array_atomic.cpp)
	add_test_generic(make_persistent_array_atomic none)
	add_test_generic(make_persistent_array_atomic pmemcheck)
else()
	message(WARNING "Skipping array tests because of clang template bug")
	skip_test("make_persistent_array" "SKIPPED_BECAUSE_OF_CLANG_TEMPLATE_BUG")
endif()

build_test(array_algorithms array_algorithms/array_algorithms.cpp)
add_test_generic(array_algorithms none)
add_test_generic(array_algorithms pmemcheck)

build_test(array_slice array_slice/array_slice.cpp)
add_test_generic(array_slice none)
add_test_generic(array_slice pmemcheck)
add_test_generic(array_slice memcheck)

build_test(array_iterator array_iterator/array_iterator.cpp)
add_test_generic(array_iterator none)
add_test_generic(array_iterator pmemcheck)

build_test(iterator_traits iterator_traits/iterator_traits.cpp)
add_test_generic(iterator_traits none)

build_test(ctl ctl/ctl.cpp)
add_test_generic(ctl none)

if(WIN32)
	build_test(ctl_win ctl_win/ctl_win.cpp)
	add_test_generic(ctl_win none)
endif()

if(PMREORDER_SUPPORTED)
	build_test(array_slice_pmreorder array_slice_pmreorder/array_slice_pmreorder.cpp)
	add_test_generic(array_slice_pmreorder none 0)
	add_test_generic(array_slice_pmreorder none 1)
else()
	message(WARNING "Skipping pmreorder tests because of no pmreorder support")
endif()

build_test(vector_comp_operators vector_comp_operators/vector_comp_operators.cpp)
add_test_generic(vector_comp_operators none)
add_test_generic(vector_comp_operators memcheck)
add_test_generic(vector_comp_operators pmemcheck)

build_test(vector_private vector_private/vector_private.cpp)
add_test_generic(vector_private none)
add_test_generic(vector_private memcheck)
add_test_generic(vector_private pmemcheck)

build_test(vector_capacity_exceptions_length vector_capacity_exceptions_length/vector_capacity_exceptions_length.cpp)
add_test_generic(vector_capacity_exceptions_length none)
add_test_generic(vector_capacity_exceptions_length memcheck)
add_test_generic(vector_capacity_exceptions_length pmemcheck)

build_test(vector_capacity_exceptions_oom vector_capacity_exceptions_oom/vector_capacity_exceptions_oom.cpp)
add_test_generic(vector_capacity_exceptions_oom none)
add_test_generic(vector_capacity_exceptions_oom memcheck)
add_test_generic(vector_capacity_exceptions_oom pmemcheck)

build_test(vector_capacity_txabort vector_capacity_txabort/vector_capacity_txabort.cpp)
add_test_generic(vector_capacity_txabort none)
add_test_generic(vector_capacity_txabort memcheck)
add_test_generic(vector_capacity_txabort pmemcheck)

build_test(vector_ctor_exceptions_nopmem vector_ctor_exceptions_nopmem/vector_ctor_exceptions_nopmem.cpp)
add_test_generic(vector_ctor_exceptions_nopmem none)
add_test_generic(vector_ctor_exceptions_nopmem memcheck)

build_test(vector_ctor_exceptions_notx vector_ctor_exceptions_notx/vector_ctor_exceptions_notx.cpp)
add_test_generic(vector_ctor_exceptions_notx none)
add_test_generic(vector_ctor_exceptions_notx memcheck)

build_test(vector_ctor_exceptions_oom vector_ctor_exceptions_oom/vector_ctor_exceptions_oom.cpp)
add_test_generic(vector_ctor_exceptions_oom none)
add_test_generic(vector_ctor_exceptions_oom memcheck)
add_test_generic(vector_ctor_exceptions_oom pmemcheck)

build_test(vector_ctor_move vector_ctor_move/vector_ctor_move.cpp)
add_test_generic(vector_ctor_move none)
add_test_generic(vector_ctor_move memcheck)
add_test_generic(vector_ctor_move pmemcheck)

build_test(vector_ctor_capacity vector_ctor_capacity/vector_ctor_capacity.cpp)
add_test_generic(vector_ctor_capacity none)
add_test_generic(vector_ctor_capacity memcheck)
add_test_generic(vector_ctor_capacity pmemcheck)

build_test(vector_dtor vector_dtor/vector_dtor.cpp)
add_test_generic(vector_dtor none)
add_test_generic(vector_dtor memcheck)
add_test_generic(vector_dtor pmemcheck)

build_test(vector_iterators_access vector_iterators_access/vector_iterators_access.cpp)
add_test_generic(vector_iterators_access none)
add_test_generic(vector_iterators_access memcheck)
add_test_generic(vector_iterators_access pmemcheck)

build_test(vector_ctor_check_copy vector_ctor_check_copy/vector_ctor_check_copy.cpp)
add_test_generic(vector_ctor_check_copy none)
add_test_generic(vector_ctor_check_copy memcheck)
add_test_generic(vector_ctor_check_copy pmemcheck)

build_test(vector_modifiers_exceptions_oom vector_modifiers_exceptions_oom/vector_modifiers_exceptions_oom.cpp)
add_test_generic(vector_modifiers_exceptions_oom none)
add_test_generic(vector_modifiers_exceptions_oom memcheck)
add_test_generic(vector_modifiers_exceptions_oom pmemcheck)

build_test(vector_modifiers_txabort vector_modifiers_txabort/vector_modifiers_txabort.cpp)
add_test_generic(vector_modifiers_txabort none)
add_test_generic(vector_modifiers_txabort memcheck)
add_test_generic(vector_modifiers_txabort pmemcheck)

add_subdirectory(external)
