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

# Utility rule file for cstyle-tests-all.

# Include any custom commands dependencies for this target.
include tests/CMakeFiles/cstyle-tests-all.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/cstyle-tests-all.dir/progress.make

tests/CMakeFiles/cstyle-tests-all: cstyle-tests-all-status

cstyle-tests-all-status: ../tests/*/*.[ch]
cstyle-tests-all-status: ../tests/*.[ch]
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ../cstyle-tests-all-status"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests && /usr/bin/perl /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/cstyle -pP -o src2man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/tests/*/*.[ch] /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/tests/*.[ch]
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests && /home/nvm-admin/cmake-3.20.2-linux-x86_64/bin/cmake -E touch /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/cstyle-tests-all-status

cstyle-tests-all: cstyle-tests-all-status
cstyle-tests-all: tests/CMakeFiles/cstyle-tests-all
cstyle-tests-all: tests/CMakeFiles/cstyle-tests-all.dir/build.make
.PHONY : cstyle-tests-all

# Rule to build all files generated by this target.
tests/CMakeFiles/cstyle-tests-all.dir/build: cstyle-tests-all
.PHONY : tests/CMakeFiles/cstyle-tests-all.dir/build

tests/CMakeFiles/cstyle-tests-all.dir/clean:
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/cstyle-tests-all.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/cstyle-tests-all.dir/clean

tests/CMakeFiles/cstyle-tests-all.dir/depend:
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/tests /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/tests/CMakeFiles/cstyle-tests-all.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/cstyle-tests-all.dir/depend

