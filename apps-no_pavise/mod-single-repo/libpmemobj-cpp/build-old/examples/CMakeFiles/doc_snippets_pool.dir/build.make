# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/swapnilh/pmdk-stuff/libpmemobj-cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/swapnilh/pmdk-stuff/libpmemobj-cpp/build

# Include any dependencies generated for this target.
include examples/CMakeFiles/doc_snippets_pool.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/doc_snippets_pool.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/doc_snippets_pool.dir/flags.make

examples/CMakeFiles/doc_snippets_pool.dir/doc_snippets/pool.cpp.o: examples/CMakeFiles/doc_snippets_pool.dir/flags.make
examples/CMakeFiles/doc_snippets_pool.dir/doc_snippets/pool.cpp.o: ../examples/doc_snippets/pool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/swapnilh/pmdk-stuff/libpmemobj-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/doc_snippets_pool.dir/doc_snippets/pool.cpp.o"
	cd /home/swapnilh/pmdk-stuff/libpmemobj-cpp/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doc_snippets_pool.dir/doc_snippets/pool.cpp.o -c /home/swapnilh/pmdk-stuff/libpmemobj-cpp/examples/doc_snippets/pool.cpp

examples/CMakeFiles/doc_snippets_pool.dir/doc_snippets/pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doc_snippets_pool.dir/doc_snippets/pool.cpp.i"
	cd /home/swapnilh/pmdk-stuff/libpmemobj-cpp/build/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/swapnilh/pmdk-stuff/libpmemobj-cpp/examples/doc_snippets/pool.cpp > CMakeFiles/doc_snippets_pool.dir/doc_snippets/pool.cpp.i

examples/CMakeFiles/doc_snippets_pool.dir/doc_snippets/pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doc_snippets_pool.dir/doc_snippets/pool.cpp.s"
	cd /home/swapnilh/pmdk-stuff/libpmemobj-cpp/build/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/swapnilh/pmdk-stuff/libpmemobj-cpp/examples/doc_snippets/pool.cpp -o CMakeFiles/doc_snippets_pool.dir/doc_snippets/pool.cpp.s

doc_snippets_pool: examples/CMakeFiles/doc_snippets_pool.dir/doc_snippets/pool.cpp.o
doc_snippets_pool: examples/CMakeFiles/doc_snippets_pool.dir/build.make

.PHONY : doc_snippets_pool

# Rule to build all files generated by this target.
examples/CMakeFiles/doc_snippets_pool.dir/build: doc_snippets_pool

.PHONY : examples/CMakeFiles/doc_snippets_pool.dir/build

examples/CMakeFiles/doc_snippets_pool.dir/clean:
	cd /home/swapnilh/pmdk-stuff/libpmemobj-cpp/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/doc_snippets_pool.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/doc_snippets_pool.dir/clean

examples/CMakeFiles/doc_snippets_pool.dir/depend:
	cd /home/swapnilh/pmdk-stuff/libpmemobj-cpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/swapnilh/pmdk-stuff/libpmemobj-cpp /home/swapnilh/pmdk-stuff/libpmemobj-cpp/examples /home/swapnilh/pmdk-stuff/libpmemobj-cpp/build /home/swapnilh/pmdk-stuff/libpmemobj-cpp/build/examples /home/swapnilh/pmdk-stuff/libpmemobj-cpp/build/examples/CMakeFiles/doc_snippets_pool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/doc_snippets_pool.dir/depend

