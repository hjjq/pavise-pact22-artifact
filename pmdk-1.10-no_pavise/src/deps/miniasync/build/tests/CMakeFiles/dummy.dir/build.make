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
include tests/CMakeFiles/dummy.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/dummy.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/dummy.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/dummy.dir/flags.make

tests/CMakeFiles/dummy.dir/dummy/dummy.c.o: tests/CMakeFiles/dummy.dir/flags.make
tests/CMakeFiles/dummy.dir/dummy/dummy.c.o: ../tests/dummy/dummy.c
tests/CMakeFiles/dummy.dir/dummy/dummy.c.o: tests/CMakeFiles/dummy.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/dummy.dir/dummy/dummy.c.o"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/dummy.dir/dummy/dummy.c.o -MF CMakeFiles/dummy.dir/dummy/dummy.c.o.d -o CMakeFiles/dummy.dir/dummy/dummy.c.o -c /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/tests/dummy/dummy.c

tests/CMakeFiles/dummy.dir/dummy/dummy.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dummy.dir/dummy/dummy.c.i"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/tests/dummy/dummy.c > CMakeFiles/dummy.dir/dummy/dummy.c.i

tests/CMakeFiles/dummy.dir/dummy/dummy.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dummy.dir/dummy/dummy.c.s"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/tests/dummy/dummy.c -o CMakeFiles/dummy.dir/dummy/dummy.c.s

# Object files for target dummy
dummy_OBJECTS = \
"CMakeFiles/dummy.dir/dummy/dummy.c.o"

# External object files for target dummy
dummy_EXTERNAL_OBJECTS =

out/dummy: tests/CMakeFiles/dummy.dir/dummy/dummy.c.o
out/dummy: tests/CMakeFiles/dummy.dir/build.make
out/dummy: out/libminiasync.so.0
out/dummy: src/libcores.a
out/dummy: tests/CMakeFiles/dummy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../out/dummy"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dummy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/dummy.dir/build: out/dummy
.PHONY : tests/CMakeFiles/dummy.dir/build

tests/CMakeFiles/dummy.dir/clean:
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/dummy.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/dummy.dir/clean

tests/CMakeFiles/dummy.dir/depend:
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/tests /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests/CMakeFiles/dummy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/dummy.dir/depend
