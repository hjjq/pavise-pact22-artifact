# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/nvm-admin/cmake-3.20.2-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/nvm-admin/cmake-3.20.2-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/memcpy_threads.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/memcpy_threads.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/memcpy_threads.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/memcpy_threads.dir/flags.make

tests/CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.o: tests/CMakeFiles/memcpy_threads.dir/flags.make
tests/CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.o: ../tests/memcpy_threads/memcpy_threads.c
tests/CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.o: tests/CMakeFiles/memcpy_threads.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.o"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.o -MF CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.o.d -o CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.o -c /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/tests/memcpy_threads/memcpy_threads.c

tests/CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.i"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/tests/memcpy_threads/memcpy_threads.c > CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.i

tests/CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.s"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/tests/memcpy_threads/memcpy_threads.c -o CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.s

# Object files for target memcpy_threads
memcpy_threads_OBJECTS = \
"CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.o"

# External object files for target memcpy_threads
memcpy_threads_EXTERNAL_OBJECTS =

out/memcpy_threads: tests/CMakeFiles/memcpy_threads.dir/memcpy_threads/memcpy_threads.c.o
out/memcpy_threads: tests/CMakeFiles/memcpy_threads.dir/build.make
out/memcpy_threads: out/libminiasync.so.0
out/memcpy_threads: src/libcores.a
out/memcpy_threads: tests/CMakeFiles/memcpy_threads.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../out/memcpy_threads"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/memcpy_threads.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/memcpy_threads.dir/build: out/memcpy_threads
.PHONY : tests/CMakeFiles/memcpy_threads.dir/build

tests/CMakeFiles/memcpy_threads.dir/clean:
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/memcpy_threads.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/memcpy_threads.dir/clean

tests/CMakeFiles/memcpy_threads.dir/depend:
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/tests /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests/CMakeFiles/memcpy_threads.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/memcpy_threads.dir/depend

