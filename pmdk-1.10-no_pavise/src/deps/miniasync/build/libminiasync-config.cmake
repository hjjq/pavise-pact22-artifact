#
# SPDX-License-Identifier: BSD-3-Clause
# Copyright 2021, Intel Corporation
#


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was libminiasync-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

find_path(MINIASYNC_INCLUDE_DIR libminiasync.h)
find_library(MINIASYNC_LIBRARY NAMES miniasync libminiasync)

set_and_check(MINIASYNC_INCLUDE "${PACKAGE_PREFIX_DIR}/include")

set(MINIASYNC_LIBRARIES ${MINIASYNC_LIBRARY})
set(MINIASYNC_INCLUDE_DIRS ${MINIASYNC_INCLUDE_DIR})
