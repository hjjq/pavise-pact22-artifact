# Copyright 2014-2018, Intel Corporation
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

#
# src/libpmem/Makefile -- Makefile for libpmem
#

include ../common.inc

LIBRARY_NAME = pmem
LIBRARY_SO_VERSION = 1
LIBRARY_VERSION = 0.0
SOURCE =\
	$(COMMON)/file.c\
	$(COMMON)/file_posix.c\
	$(COMMON)/fs_posix.c\
	$(COMMON)/mmap.c\
	$(COMMON)/mmap_posix.c\
	$(COMMON)/os_posix.c\
	$(COMMON)/os_thread_posix.c\
	$(COMMON)/os_deep_linux.c\
	$(COMMON)/os_auto_flush_linux.c\
	$(COMMON)/out.c\
	$(COMMON)/util.c\
	$(COMMON)/util_posix.c\
	libpmem.c\
	memops_generic.c\
	pmem.c\
	pmem_posix.c

include $(ARCH)/sources.inc

SOURCE += $(LIBPMEM_ARCH_SOURCE)

include ../Makefile.inc

include $(ARCH)/flags.inc

CFLAGS += -I. #-mclwb
LIBS += -pthread
