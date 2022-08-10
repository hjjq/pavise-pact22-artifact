# From: https://cmake.org/Wiki/CMake_FAQ

if(NOT EXISTS "/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: /home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/install_manifest.txt")
endif()

file(READ "/home/kevin/pavise/pavise-pact22-tmp/pmdk-1.10-no_pavise/src/deps/miniasync/build/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
	message(STATUS "Uninstalling $ENV{DESTDIR}${file}")
	if(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
		exec_program(
			"/home/nvm-admin/cmake-3.20.2-linux-x86_64/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
			OUTPUT_VARIABLE rm_out
			RETURN_VALUE rm_retval)
		if(NOT "${rm_retval}" STREQUAL 0)
			message(FATAL_ERROR "Problem when removing $ENV{DESTDIR}${file}")
		endif()
	else(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
		message(STATUS "File $ENV{DESTDIR}${file} does not exist.")
	endif()
endforeach()
