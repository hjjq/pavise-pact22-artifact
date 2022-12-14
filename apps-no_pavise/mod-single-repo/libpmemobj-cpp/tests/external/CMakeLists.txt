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

include(../ctest_helpers.cmake)

function(add_test_default name tracer)
	if ("${ARGN}" STREQUAL "")
		set(testcase "0")
	else()
		set(testcase "${ARGN}")
	endif()

	set(cmake_script ${CMAKE_CURRENT_SOURCE_DIR}/run_default.cmake)

	add_test_common(${name} ${tracer} ${testcase} ${cmake_script})
endfunction()

function(add_test_expect_failure name)
	set(srcs ${ARGN})
	prepend(srcs ${CMAKE_CURRENT_SOURCE_DIR} ${srcs})

	add_cppstyle(tests-external-${name} ${srcs})
	add_check_whitespace(tests-external-${name} ${srcs})

	add_executable(${name}_fail_expected ${srcs})
	set_target_properties(${name}_fail_expected PROPERTIES
				EXCLUDE_FROM_ALL TRUE
				EXCLUDE_FROM_DEFAULT_BUILD TRUE)

	add_test(NAME ${name}_fail_expected
		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${name}_fail_expected
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

	set_tests_properties(${name}_fail_expected PROPERTIES WILL_FAIL TRUE)
endfunction()

build_test(array_at libcxx/array/at.pass.cpp)
add_test_default(array_at none)

build_test(array_begin libcxx/array/begin.pass.cpp)
add_test_default(array_begin none)

build_test(array_compare libcxx/array/compare.pass.cpp)
add_test_default(array_compare none)

build_test(array_contiguous libcxx/array/contiguous.pass.cpp)
add_test_default(array_contiguous none)

build_test(array_empty libcxx/array/empty.pass.cpp)
add_test_default(array_empty none)

build_test(array_front_back libcxx/array/front_back.pass.cpp)
add_test_default(array_front_back none)

build_test(array_indexing libcxx/array/indexing.pass.cpp)
add_test_default(array_indexing none)

build_test(array_max_size libcxx/array/max_size.pass.cpp)
add_test_default(array_max_size none)

if (MAX_ALIGN_TYPE_EXISTS)
	build_test(array_size_and_alignment libcxx/array/size_and_alignment.pass.cpp)
	add_test_default(array_size_and_alignment none)
endif()

build_test(array_types libcxx/array/types.pass.cpp)
add_test_default(array_types none)

add_test_expect_failure(array_compare_0 libcxx/array/compare.fail_0.cpp)
add_test_expect_failure(array_compare_1 libcxx/array/compare.fail_1.cpp)
add_test_expect_failure(array_compare_2 libcxx/array/compare.fail_2.cpp)

build_test(array_cons_default libcxx/array/array.cons/default.pass.cpp)
add_test_default(array_cons_default none)

add_test_expect_failure(array_copy_assignment libcxx/array/array.cons/copy_assignment.fail.cpp)

build_test(array_cons_implicit_copy libcxx/array/array.cons/implicit_copy.pass.cpp)
add_test_default(array_cons_implicit_copy none)

build_test(array_initializer_list libcxx/array/array.cons/initializer_list.pass.cpp)
add_test_default(array_initializer_list none)

if (MAX_ALIGN_TYPE_EXISTS)
	build_test(array_data_const libcxx/array/array.data/data_const.pass.cpp)
	add_test_default(array_data_const none)
endif()

if (MAX_ALIGN_TYPE_EXISTS)
	build_test(array_data libcxx/array/array.data/data.pass.cpp)
	add_test_default(array_data none)
endif()

add_test_expect_failure(array_fill libcxx/array/array.fill/fill.fail.cpp)

build_test(array_fill libcxx/array/array.fill/fill.pass.cpp)
add_test_default(array_fill none)

build_test(array_size libcxx/array/array.size/size.pass.cpp)
add_test_default(array_size none)

build_test(array_swap libcxx/array/array.special/swap.pass.cpp)
add_test_default(array_swap none)

add_test_expect_failure(array_swap libcxx/array/array.swap/swap.fail.cpp)

build_test(array_swap_2 libcxx/array/array.swap/swap.pass.cpp)
add_test_default(array_swap_2 none)

build_test(array_get_const_rv libcxx/array/array.tuple/get_const_rv.pass.cpp)
add_test_default(array_get_const_rv none)

build_test(array_get_const libcxx/array/array.tuple/get_const.pass.cpp)
add_test_default(array_get_const none)

build_test(array_get_rv libcxx/array/array.tuple/get_rv.pass.cpp)
add_test_default(array_get_rv none)

build_test(array_get libcxx/array/array.tuple/get.pass.cpp)
add_test_default(array_get none)

add_test_expect_failure(array_get libcxx/array/array.tuple/get.fail.cpp)

build_test(array_iterators libcxx/array/iterators.pass.cpp)
add_test_default(array_iterators none)

build_test(vector_libcxx_ctor_default libcxx/vector/vector.cons/construct_default.pass.cpp)
add_test_default(vector_libcxx_ctor_default none)
add_test_default(vector_libcxx_ctor_default pmemcheck)
add_test_default(vector_libcxx_ctor_default memcheck)

build_test(vector_libcxx_ctor_iter_iter libcxx/vector/vector.cons/construct_iter_iter.pass.cpp)
add_test_default(vector_libcxx_ctor_iter_iter none)
add_test_default(vector_libcxx_ctor_iter_iter pmemcheck)
add_test_default(vector_libcxx_ctor_iter_iter memcheck)

build_test(vector_libcxx_ctor_size libcxx/vector/vector.cons/construct_size.pass.cpp)
add_test_default(vector_libcxx_ctor_size none)
add_test_default(vector_libcxx_ctor_size pmemcheck)
add_test_default(vector_libcxx_ctor_size memcheck)

build_test(vector_libcxx_ctor_size_value libcxx/vector/vector.cons/construct_size_value.pass.cpp)
add_test_default(vector_libcxx_ctor_size_value none)
add_test_default(vector_libcxx_ctor_size_value pmemcheck)
add_test_default(vector_libcxx_ctor_size_value memcheck)

build_test(vector_libcxx_copy libcxx/vector/vector.cons/copy.pass.cpp)
add_test_default(vector_libcxx_copy none)
add_test_default(vector_libcxx_copy pmemcheck)
add_test_default(vector_libcxx_copy memcheck)

build_test(vector_libcxx_dtor_noexcept libcxx/vector/vector.cons/dtor_noexcept.pass.cpp)
add_test_default(vector_libcxx_dtor_noexcept none)

build_test(vector_libcxx_contiguous libcxx/vector/contiguous.pass.cpp)
add_test_default(vector_libcxx_contiguous none)
add_test_default(vector_libcxx_contiguous pmemcheck)
add_test_default(vector_libcxx_contiguous memcheck)

build_test(vector_libcxx_types libcxx/vector/types.pass.cpp)
add_test_default(vector_libcxx_types none)

build_test(vector_libcxx_iterators libcxx/vector/iterators.pass.cpp)
add_test_default(vector_libcxx_iterators none)
add_test_default(vector_libcxx_iterators pmemcheck)
add_test_default(vector_libcxx_iterators memcheck)

build_test(vector_libcxx_data_const libcxx/vector/vector.data/data_const.pass.cpp)
add_test_default(vector_libcxx_data_const none)
add_test_default(vector_libcxx_data_const pmemcheck)
add_test_default(vector_libcxx_data_const memcheck)

build_test(vector_libcxx_data libcxx/vector/vector.data/data.pass.cpp)
add_test_default(vector_libcxx_data none)
add_test_default(vector_libcxx_data pmemcheck)
add_test_default(vector_libcxx_data memcheck)

build_test(vector_libcxx_initializer_list libcxx/vector/vector.cons/initializer_list.pass.cpp)
add_test_default(vector_libcxx_initializer_list none)
add_test_default(vector_libcxx_initializer_list pmemcheck)
add_test_default(vector_libcxx_initializer_list memcheck)

# XXX: enable vector_libcxx_capacity test when vector's push_back method is implemented
#build_test(vector_libcxx_capacity libcxx/vector/vector.capacity/capacity.pass.cpp)
#add_test_default(vector_libcxx_capacity none)
#add_test_default(vector_libcxx_capacity pmemcheck)
#add_test_default(vector_libcxx_capacity memcheck)

# XXX: enable vector_libcxx_empty test when vector's push_back method is implemented
#build_test(vector_libcxx_empty libcxx/vector/vector.capacity/empty.pass.cpp)
#add_test_default(vector_libcxx_empty none)
#add_test_default(vector_libcxx_empty pmemcheck)
#add_test_default(vector_libcxx_empty memcheck)

build_test(vector_libcxx_max_size libcxx/vector/vector.capacity/max_size.pass.cpp)
add_test_default(vector_libcxx_max_size none)
add_test_default(vector_libcxx_max_size pmemcheck)
add_test_default(vector_libcxx_max_size memcheck)

build_test(vector_libcxx_reserve libcxx/vector/vector.capacity/reserve.pass.cpp)
add_test_default(vector_libcxx_reserve none)
add_test_default(vector_libcxx_reserve pmemcheck)
add_test_default(vector_libcxx_reserve memcheck)

build_test(vector_libcxx_resize_size libcxx/vector/vector.capacity/resize_size.pass.cpp)
add_test_default(vector_libcxx_resize_size none)
add_test_default(vector_libcxx_resize_size pmemcheck)
add_test_default(vector_libcxx_resize_size memcheck)

build_test(vector_libcxx_resize_size_value libcxx/vector/vector.capacity/resize_size_value.pass.cpp)
add_test_default(vector_libcxx_resize_size_value none)
add_test_default(vector_libcxx_resize_size_value pmemcheck)
add_test_default(vector_libcxx_resize_size_value memcheck)

# XXX: enable vector_libcxx_shrink_to_fit test when vector's push_back method is implemented
#build_test(vector_libcxx_shrink_to_fit libcxx/vector/vector.capacity/shrink_to_fit.pass.cpp)
#add_test_default(vector_libcxx_shrink_to_fit none)
#add_test_default(vector_libcxx_shrink_to_fit pmemcheck)
#add_test_default(vector_libcxx_shrink_to_fit memcheck)

# XXX: enable vector_libcxx_size test when vector's push_back and erase methods are implemented
#build_test(vector_libcxx_size libcxx/vector/vector.capacity/size.pass.cpp)
#add_test_default(vector_libcxx_size none)
#add_test_default(vector_libcxx_size pmemcheck)
#add_test_default(vector_libcxx_size memcheck)

build_test(vector_libcxx_swap libcxx/vector/vector.capacity/swap.pass.cpp)
add_test_default(vector_libcxx_swap none)
add_test_default(vector_libcxx_swap pmemcheck)
add_test_default(vector_libcxx_swap memcheck)

# XXX: port libcxx test vector.cons/assign_copy.pass

# XXX: port libcxx test vector.cons/assign_initializer_list.pass

# XXX: port libcxx test vector.cons/assign_iter_iter.pass

# XXX: port libcxx test vector.cons/assign_move.pass

# XXX: port libcxx test vector.cons/assign_size_value.pass

# XXX: port libcxx test vector.cons/default_noexcept.pass

# XXX: port libcxx test vector.cons/default_recursive.pass

# XXX: port libcxx test vector.cons/move_assign_noexcept.pass

# XXX: port libcxx test vector.cons/move_noexcept.pass

# XXX: port libcxx test vector.cons/move.pass

# XXX: port libcxx test vector.cons/op_equal_initializer_list.pass

build_test(vector_libcxx_clear libcxx/vector/vector.modifiers/clear.pass.cpp)
add_test_default(vector_libcxx_clear none)
add_test_default(vector_libcxx_clear pmemcheck)
add_test_default(vector_libcxx_clear memcheck)

# XXX: port libcxx test vector.modifiers/emplace_back.pass

# XXX: port libcxx test vector.modifiers/emplace_extra.pass

# XXX: port libcxx test vector.modifiers/emplace.pass

# XXX: port libcxx test vector.modifiers/erase_iter_iter.pass

# XXX: port libcxx test vector.modifiers/erase_iter.pass

# XXX: port libcxx test vector.modifiers/insert_iter_initializer_list.pass

# XXX: port libcxx test vector.modifiers/insert_iter_iter_iter.pass

# XXX: port libcxx test vector.modifiers/insert_iter_rvalue.pass

# XXX: port libcxx test vector.modifiers/insert_iter_size_value.pass

# XXX: port libcxx test vector.modifiers/insert_iter_value.pass

# XXX: port libcxx test vector.modifiers/pop_back.pass

# XXX: port libcxx test vector.modifiers/push_back_exception_safety.pass

# XXX: port libcxx test vector.modifiers/push_back_rvalue.pass

# XXX: port libcxx test vector.modifiers/push_back.pass

# XXX: port libcxx test vector.special/swap_noexcept.pass

# XXX: port libcxx test vector.special/swap.pass

# XXX: port libcxx test basic_string/string.starts_with/starts_with.string_view.pass
# XXX: port libcxx test basic_string/string.starts_with/starts_with.ptr.pass
# XXX: port libcxx test basic_string/string.starts_with/starts_with.char.pass
# XXX: port libcxx test basic_string/test_trai
# XXX: port libcxx test basic_string/string.ends_with/ends_with.ptr.pass
# XXX: port libcxx test basic_string/string.ends_with/ends_with.string_view.pass
# XXX: port libcxx test basic_string/string.ends_with/ends_with.char.pass
# XXX: port libcxx test basic_string/string.access/db_front.pass
# XXX: port libcxx test basic_string/string.access/db_cindex.pass
# XXX: port libcxx test basic_string/string.access/at.pass
# XXX: port libcxx test basic_string/string.access/index.pass
# XXX: port libcxx test basic_string/string.access/back.pass
# XXX: port libcxx test basic_string/string.access/db_back.pass
# XXX: port libcxx test basic_string/string.access/db_index.pass
# XXX: port libcxx test basic_string/string.access/front.pass
# XXX: port libcxx test basic_string/string.access/db_cback.pass
# XXX: port libcxx test basic_string/string.access/db_cfront.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_oplt=/pointer_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_oplt=/string_pointer.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_oplt=/string_string_view.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_oplt=/string_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_oplt=/string_view_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_operator==/pointer_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_operator==/string_pointer.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_operator==/string_string_view.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_operator==/string_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_operator==/string_view_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string.special/swap.pass
# XXX: port libcxx test basic_string/string.nonmembers/string.special/swap_noexcept.pass
# XXX: port libcxx test basic_string/string.nonmembers/string.io/stream_extract.pass
# XXX: port libcxx test basic_string/string.nonmembers/string.io/stream_insert.pass
# XXX: port libcxx test basic_string/string.nonmembers/string.io/get_line.pass
# XXX: port libcxx test basic_string/string.nonmembers/string.io/get_line_delim_rv.pass
# XXX: port libcxx test basic_string/string.nonmembers/string.io/get_line_delim.pass
# XXX: port libcxx test basic_string/string.nonmembers/string.io/get_line_rv.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_op+/pointer_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_op+/string_pointer.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_op+/char_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_op+/string_char.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_op+/string_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_oplt/pointer_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_oplt/string_pointer.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_oplt/string_string_view.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_oplt/string_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_oplt/string_view_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_op!=/pointer_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_op!=/string_pointer.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_op!=/string_string_view.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_op!=/string_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_op!=/string_view_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_opgt/pointer_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_opgt/string_pointer.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_opgt/string_string_view.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_opgt/string_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_opgt/string_view_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_opgt=/pointer_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_opgt=/string_pointer.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_opgt=/string_string_view.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_opgt=/string_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/string_opgt=/string_view_string.pass
# XXX: port libcxx test basic_string/string.nonmembers/nothing_to_do.pass
# XXX: port libcxx test basic_string/string.ops/string_find.last.of/string_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.last.of/string_view_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.last.of/pointer_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.last.of/char_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.last.of/pointer_size_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.last.not.of/string_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.last.not.of/string_view_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.last.not.of/pointer_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.last.not.of/char_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.last.not.of/pointer_size_size.pass
# XXX: port libcxx test basic_string/string.ops/string_compare/size_size_string_size_size.pass
# XXX: port libcxx test basic_string/string.ops/string_compare/size_size_string.pass
# XXX: port libcxx test basic_string/string.ops/string_compare/size_size_string_view.pass
# XXX: port libcxx test basic_string/string.ops/string_compare/size_size_T_size_size.pass
# XXX: port libcxx test basic_string/string.ops/string_compare/size_size_pointer.pass
# XXX: port libcxx test basic_string/string.ops/string_compare/string_view.pass
# XXX: port libcxx test basic_string/string.ops/string_compare/string.pass
# XXX: port libcxx test basic_string/string.ops/string_compare/pointer.pass
# XXX: port libcxx test basic_string/string.ops/string_compare/size_size_pointer_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.first.not.of/string_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.first.not.of/string_view_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.first.not.of/pointer_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.first.not.of/char_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.first.not.of/pointer_size_size.pass
# XXX: port libcxx test basic_string/string.ops/string_rfind/string_size.pass
# XXX: port libcxx test basic_string/string.ops/string_rfind/string_view_size.pass
# XXX: port libcxx test basic_string/string.ops/string_rfind/pointer_size.pass
# XXX: port libcxx test basic_string/string.ops/string_rfind/char_size.pass
# XXX: port libcxx test basic_string/string.ops/string_rfind/pointer_size_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.first.of/string_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.first.of/string_view_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.first.of/pointer_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.first.of/char_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find.first.of/pointer_size_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find/string_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find/string_view_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find/pointer_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find/char_size.pass
# XXX: port libcxx test basic_string/string.ops/string_find/pointer_size_size.pass
# XXX: port libcxx test basic_string/string.ops/nothing_to_do.pass
# XXX: port libcxx test basic_string/string.ops/string_substr/substr.pass
# XXX: port libcxx test basic_string/string.ops/string.accessors/get_allocator.pass
# XXX: port libcxx test basic_string/string.ops/string.accessors/c_str.pass
# XXX: port libcxx test basic_string/string.ops/string.accessors/data.pass
# XXX: port libcxx test basic_string/types.pass
# XXX: port libcxx test basic_string/traits_mismatch.fail
# XXX: port libcxx test basic_string/string.modifiers/string_swap/swap.pass
# XXX: port libcxx test basic_string/string.modifiers/string_erase/iter_iter.pass
# XXX: port libcxx test basic_string/string.modifiers/string_erase/size_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_erase/iter.pass
# XXX: port libcxx test basic_string/string.modifiers/string_erase/pop_back.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/size_size_size_char.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/iter_iter_iter_iter.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/size_size_string_size_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/size_size_string.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/size_size_string_view.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/size_size_T_size_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/iter_iter_string_view.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/iter_iter_initializer_list.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/iter_iter_string.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/iter_iter_pointer.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/size_size_pointer.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/iter_iter_pointer_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/iter_iter_size_char.pass
# XXX: port libcxx test basic_string/string.modifiers/string_replace/size_size_pointer_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_op_plus_equal/char.pass
# XXX: port libcxx test basic_string/string.modifiers/string_op_plus_equal/string.pass
# XXX: port libcxx test basic_string/string.modifiers/string_op_plus_equal/initializer_list.pass
# XXX: port libcxx test basic_string/string.modifiers/string_op_plus_equal/pointer.pass
# XXX: port libcxx test basic_string/string.modifiers/string_copy/copy.pass
# XXX: port libcxx test basic_string/string.modifiers/string_append/push_back.pass
# XXX: port libcxx test basic_string/string.modifiers/string_append/iterator.pass
# XXX: port libcxx test basic_string/string.modifiers/string_append/T_size_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_append/string_size_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_append/pointer_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_append/size_char.pass
# XXX: port libcxx test basic_string/string.modifiers/string_append/string_view.pass
# XXX: port libcxx test basic_string/string.modifiers/string_append/string.pass
# XXX: port libcxx test basic_string/string.modifiers/string_append/initializer_list.pass
# XXX: port libcxx test basic_string/string.modifiers/string_append/pointer.pass
# XXX: port libcxx test basic_string/string.modifiers/string_insert/iter_char.pass
# XXX: port libcxx test basic_string/string.modifiers/string_insert/iter_initializer_list.pass
# XXX: port libcxx test basic_string/string.modifiers/string_insert/size_string_size_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_insert/size_string.pass
# XXX: port libcxx test basic_string/string.modifiers/string_insert/iter_size_char.pass
# XXX: port libcxx test basic_string/string.modifiers/string_insert/size_size_char.pass
# XXX: port libcxx test basic_string/string.modifiers/string_insert/size_pointer.pass
# XXX: port libcxx test basic_string/string.modifiers/string_insert/size_pointer_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_insert/iter_iter_iter.pass
# XXX: port libcxx test basic_string/string.modifiers/string_insert/string_view.pass
# XXX: port libcxx test basic_string/string.modifiers/string_insert/size_T_size_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_assign/iterator.pass
# XXX: port libcxx test basic_string/string.modifiers/string_assign/T_size_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_assign/string_size_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_assign/pointer_size.pass
# XXX: port libcxx test basic_string/string.modifiers/string_assign/rv_string.pass
# XXX: port libcxx test basic_string/string.modifiers/string_assign/size_char.pass
# XXX: port libcxx test basic_string/string.modifiers/string_assign/string_view.pass
# XXX: port libcxx test basic_string/string.modifiers/string_assign/string.pass
# XXX: port libcxx test basic_string/string.modifiers/string_assign/initializer_list.pass
# XXX: port libcxx test basic_string/string.modifiers/string_assign/pointer.pass
# XXX: port libcxx test basic_string/string.modifiers/nothing_to_do.pass
# XXX: port libcxx test basic_string/char.bad.fail
# XXX: port libcxx test basic_string/string.iterators/db_iterators_8.pass
# XXX: port libcxx test basic_string/string.iterators/begin.pass
# XXX: port libcxx test basic_string/string.iterators/cbegin.pass
# XXX: port libcxx test basic_string/string.iterators/rend.pass
# XXX: port libcxx test basic_string/string.iterators/cend.pass
# XXX: port libcxx test basic_string/string.iterators/db_iterators_5.pass
# XXX: port libcxx test basic_string/string.iterators/db_iterators_2.pass
# XXX: port libcxx test basic_string/string.iterators/crbegin.pass
# XXX: port libcxx test basic_string/string.iterators/iterators.pass
# XXX: port libcxx test basic_string/string.iterators/db_iterators_7.pass
# XXX: port libcxx test basic_string/string.iterators/db_iterators_3.pass
# XXX: port libcxx test basic_string/string.iterators/rbegin.pass
# XXX: port libcxx test basic_string/string.iterators/db_iterators_4.pass
# XXX: port libcxx test basic_string/string.iterators/crend.pass
# XXX: port libcxx test basic_string/string.iterators/end.pass
# XXX: port libcxx test basic_string/string.iterators/db_iterators_6.pass
# XXX: port libcxx test basic_string/string.require/contiguous.pass
# XXX: port libcxx test basic_string/string.capacity/clear.pass
# XXX: port libcxx test basic_string/string.capacity/resize_size.pass
# XXX: port libcxx test basic_string/string.capacity/size.pass
# XXX: port libcxx test basic_string/string.capacity/length.pass
# XXX: port libcxx test basic_string/string.capacity/reserve.pass
# XXX: port libcxx test basic_string/string.capacity/capacity.pass
# XXX: port libcxx test basic_string/string.capacity/shrink_to_fit.pass
# XXX: port libcxx test basic_string/string.capacity/empty.fail
# XXX: port libcxx test basic_string/string.capacity/empty.pass
# XXX: port libcxx test basic_string/string.capacity/resize_size_char.pass
# XXX: port libcxx test basic_string/string.capacity/max_size.pass
# XXX: port libcxx test basic_string/string.capacity/over_max_size.pass
# XXX: port libcxx test basic_string/string.cons/implicit_deduction_guides.pass
# XXX: port libcxx test basic_string/string.cons/copy_assignment.pass
# XXX: port libcxx test basic_string/string.cons/string_view_assignment.pass
# XXX: port libcxx test basic_string/string.cons/iter_alloc.pass
# XXX: port libcxx test basic_string/string.cons/T_size_size.pass
# XXX: port libcxx test basic_string/string.cons/copy_alloc.pass
# XXX: port libcxx test basic_string/string.cons/string_view_size_size_deduction.fail
# XXX: port libcxx test basic_string/string.cons/brace_assignment.pass
# XXX: port libcxx test basic_string/string.cons/string_view_size_size_deduction.pass
# XXX: port libcxx test basic_string/string.cons/move_noexcept.pass
# XXX: port libcxx test basic_string/string.cons/move_assign_noexcept.pass
# XXX: port libcxx test basic_string/string.cons/substr.pass
# XXX: port libcxx test basic_string/string.cons/iter_alloc_deduction.pass
# XXX: port libcxx test basic_string/string.cons/move.pass
# XXX: port libcxx test basic_string/string.cons/dtor_noexcept.pass
# XXX: port libcxx test basic_string/string.cons/pointer_alloc.pass
# XXX: port libcxx test basic_string/string.cons/char_assignment.pass
# XXX: port libcxx test basic_string/string.cons/string_view_deduction.fail
# XXX: port libcxx test basic_string/string.cons/pointer_size_alloc.pass
# XXX: port libcxx test basic_string/string.cons/copy.pass
# XXX: port libcxx test basic_string/string.cons/iter_alloc_deduction.fail
# XXX: port libcxx test basic_string/string.cons/string_view.pass
# XXX: port libcxx test basic_string/string.cons/initializer_list_assignment.pass
# XXX: port libcxx test basic_string/string.cons/move_alloc.pass
# XXX: port libcxx test basic_string/string.cons/pointer_assignment.pass
# XXX: port libcxx test basic_string/string.cons/initializer_list.pass
# XXX: port libcxx test basic_string/string.cons/string_view.fail
# XXX: port libcxx test basic_string/string.cons/default_noexcept.pass
# XXX: port libcxx test basic_string/string.cons/string_view_deduction.pass
# XXX: port libcxx test basic_string/string.cons/size_char_alloc.pass
# XXX: port libcxx test basic_string/string.cons/alloc.pass
# XXX: port libcxx test basic_string/string.cons/move_assignment.pass
