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

# Utility rule file for doc.

# Include any custom commands dependencies for this target.
include doc/CMakeFiles/doc.dir/compiler_depend.make

# Include the progress variables for this target.
include doc/CMakeFiles/doc.dir/progress.make

doc/CMakeFiles/doc: doc/man/data_mover_dml_get_vdm.3
doc/CMakeFiles/doc: doc/man/data_mover_dml_new.3
doc/CMakeFiles/doc: doc/man/data_mover_sync_get_vdm.3
doc/CMakeFiles/doc: doc/man/data_mover_sync_new.3
doc/CMakeFiles/doc: doc/man/data_mover_threads_get_vdm.3
doc/CMakeFiles/doc: doc/man/data_mover_threads_new.3
doc/CMakeFiles/doc: doc/man/future_context_get_data.3
doc/CMakeFiles/doc: doc/man/future_context_get_output.3
doc/CMakeFiles/doc: doc/man/future_context_get_size.3
doc/CMakeFiles/doc: doc/man/future_poll.3
doc/CMakeFiles/doc: doc/man/miniasync.7
doc/CMakeFiles/doc: doc/man/miniasync_future.7
doc/CMakeFiles/doc: doc/man/miniasync_runtime.7
doc/CMakeFiles/doc: doc/man/miniasync_vdm.7
doc/CMakeFiles/doc: doc/man/miniasync_vdm_dml.7
doc/CMakeFiles/doc: doc/man/miniasync_vdm_synchronous.7
doc/CMakeFiles/doc: doc/man/miniasync_vdm_threads.7
doc/CMakeFiles/doc: doc/man/runtime_new.3
doc/CMakeFiles/doc: doc/man/runtime_wait.3
doc/CMakeFiles/doc: doc/man/vdm_memcpy.3
doc/CMakeFiles/doc: doc/man/vdm_memmove.3
doc/CMakeFiles/doc: doc/man/vdm_memset.3

doc/man/data_mover_dml_get_vdm.3: ../doc/data_mover_dml_get_vdm.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating man/data_mover_dml_get_vdm.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/data_mover_dml_get_vdm.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/data_mover_dml_get_vdm.3 0.1.0

doc/man/data_mover_dml_new.3: ../doc/data_mover_dml_new.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating man/data_mover_dml_new.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/data_mover_dml_new.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/data_mover_dml_new.3 0.1.0

doc/man/data_mover_sync_get_vdm.3: ../doc/data_mover_sync_get_vdm.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Generating man/data_mover_sync_get_vdm.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/data_mover_sync_get_vdm.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/data_mover_sync_get_vdm.3 0.1.0

doc/man/data_mover_sync_new.3: ../doc/data_mover_sync_new.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Generating man/data_mover_sync_new.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/data_mover_sync_new.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/data_mover_sync_new.3 0.1.0

doc/man/data_mover_threads_get_vdm.3: ../doc/data_mover_threads_get_vdm.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Generating man/data_mover_threads_get_vdm.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/data_mover_threads_get_vdm.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/data_mover_threads_get_vdm.3 0.1.0

doc/man/data_mover_threads_new.3: ../doc/data_mover_threads_new.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Generating man/data_mover_threads_new.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/data_mover_threads_new.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/data_mover_threads_new.3 0.1.0

doc/man/future_context_get_data.3: ../doc/future_context_get_data.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Generating man/future_context_get_data.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/future_context_get_data.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/future_context_get_data.3 0.1.0

doc/man/future_context_get_output.3: ../doc/future_context_get_output.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Generating man/future_context_get_output.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/future_context_get_output.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/future_context_get_output.3 0.1.0

doc/man/future_context_get_size.3: ../doc/future_context_get_size.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Generating man/future_context_get_size.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/future_context_get_size.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/future_context_get_size.3 0.1.0

doc/man/future_poll.3: ../doc/future_poll.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Generating man/future_poll.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/future_poll.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/future_poll.3 0.1.0

doc/man/miniasync.7: ../doc/miniasync.7.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Generating man/miniasync.7"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/miniasync.7.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/miniasync.7 0.1.0

doc/man/miniasync_future.7: ../doc/miniasync_future.7.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Generating man/miniasync_future.7"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/miniasync_future.7.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/miniasync_future.7 0.1.0

doc/man/miniasync_runtime.7: ../doc/miniasync_runtime.7.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Generating man/miniasync_runtime.7"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/miniasync_runtime.7.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/miniasync_runtime.7 0.1.0

doc/man/miniasync_vdm.7: ../doc/miniasync_vdm.7.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Generating man/miniasync_vdm.7"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/miniasync_vdm.7.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/miniasync_vdm.7 0.1.0

doc/man/miniasync_vdm_dml.7: ../doc/miniasync_vdm_dml.7.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Generating man/miniasync_vdm_dml.7"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/miniasync_vdm_dml.7.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/miniasync_vdm_dml.7 0.1.0

doc/man/miniasync_vdm_synchronous.7: ../doc/miniasync_vdm_synchronous.7.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Generating man/miniasync_vdm_synchronous.7"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/miniasync_vdm_synchronous.7.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/miniasync_vdm_synchronous.7 0.1.0

doc/man/miniasync_vdm_threads.7: ../doc/miniasync_vdm_threads.7.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Generating man/miniasync_vdm_threads.7"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/miniasync_vdm_threads.7.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/miniasync_vdm_threads.7 0.1.0

doc/man/runtime_new.3: ../doc/runtime_new.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Generating man/runtime_new.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/runtime_new.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/runtime_new.3 0.1.0

doc/man/runtime_wait.3: ../doc/runtime_wait.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Generating man/runtime_wait.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/runtime_wait.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/runtime_wait.3 0.1.0

doc/man/vdm_memcpy.3: ../doc/vdm_memcpy.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_20) "Generating man/vdm_memcpy.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/vdm_memcpy.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/vdm_memcpy.3 0.1.0

doc/man/vdm_memmove.3: ../doc/vdm_memmove.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_21) "Generating man/vdm_memmove.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/vdm_memmove.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/vdm_memmove.3 0.1.0

doc/man/vdm_memset.3: ../doc/vdm_memset.3.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_22) "Generating man/vdm_memset.3"
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && ../../utils/md2man/md2man.sh /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc/vdm_memset.3.md /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/utils/md2man/default.man /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/man/vdm_memset.3 0.1.0

doc: doc/CMakeFiles/doc
doc: doc/man/data_mover_dml_get_vdm.3
doc: doc/man/data_mover_dml_new.3
doc: doc/man/data_mover_sync_get_vdm.3
doc: doc/man/data_mover_sync_new.3
doc: doc/man/data_mover_threads_get_vdm.3
doc: doc/man/data_mover_threads_new.3
doc: doc/man/future_context_get_data.3
doc: doc/man/future_context_get_output.3
doc: doc/man/future_context_get_size.3
doc: doc/man/future_poll.3
doc: doc/man/miniasync.7
doc: doc/man/miniasync_future.7
doc: doc/man/miniasync_runtime.7
doc: doc/man/miniasync_vdm.7
doc: doc/man/miniasync_vdm_dml.7
doc: doc/man/miniasync_vdm_synchronous.7
doc: doc/man/miniasync_vdm_threads.7
doc: doc/man/runtime_new.3
doc: doc/man/runtime_wait.3
doc: doc/man/vdm_memcpy.3
doc: doc/man/vdm_memmove.3
doc: doc/man/vdm_memset.3
doc: doc/CMakeFiles/doc.dir/build.make
.PHONY : doc

# Rule to build all files generated by this target.
doc/CMakeFiles/doc.dir/build: doc
.PHONY : doc/CMakeFiles/doc.dir/build

doc/CMakeFiles/doc.dir/clean:
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc && $(CMAKE_COMMAND) -P CMakeFiles/doc.dir/cmake_clean.cmake
.PHONY : doc/CMakeFiles/doc.dir/clean

doc/CMakeFiles/doc.dir/depend:
	cd /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/doc /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/doc/CMakeFiles/doc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : doc/CMakeFiles/doc.dir/depend

