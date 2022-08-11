/*
 * Copyright 2015-2017, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SET_NOTX_H
#define SET_NOTX_H

#include <stddef.h>
#include <stdint.h>
#include <libpmemobj.h>

struct set_args {
	uint32_t seed;
};

enum set_cmd {
	SET_CMD_REBUILD,
	SET_CMD_DEBUG,
};

#ifndef SET_NOTX_TYPE_OFFSET
#define SET_NOTX_TYPE_OFFSET 1008
#endif

struct set_notx;
struct set_entry;
struct set_buckets
;
TOID_DECLARE(struct set_notx, SET_NOTX_TYPE_OFFSET + 0);
TOID_DECLARE(struct set_buckets, SET_NOTX_TYPE_OFFSET + 1);
TOID_DECLARE(struct set_entry, SET_NOTX_TYPE_OFFSET + 2);

int set_notx_check(PMEMobjpool *pop, TOID(struct set_notx) set);
int set_notx_create(PMEMobjpool *pop, TOID(struct set_notx) *map, void *arg);
int set_notx_init(PMEMobjpool *pop, TOID(struct set_notx) set);
int set_notx_insert(PMEMobjpool *pop, TOID(struct set_notx) set,
		uint64_t key);
int set_notx_remove(PMEMobjpool *pop, TOID(struct set_notx) set,
		uint64_t key);
int set_notx_lookup(PMEMobjpool *pop, TOID(struct set_notx) set,
		uint64_t key);
int set_notx_foreach(PMEMobjpool *pop, TOID(struct set_notx) set,
	int (*cb)(uint64_t key, void *arg), void *arg);
size_t set_notx_count(PMEMobjpool *pop, TOID(struct set_notx) set);
int set_notx_cmd(PMEMobjpool *pop, TOID(struct set_notx) set,
		unsigned cmd, uint64_t arg);

struct set_entry {
	uint64_t key;

	/* next set_entry list pointer */
	TOID(struct set_entry) next;
};

struct set_buckets {
	/* number of set_buckets */
	size_t nbuckets;
	/* array of lists */
	TOID(struct set_entry) bucket[];
};

struct set_notx {
	/* random number generator seed */
	uint32_t seed;

	/* hash function coefficients */
	uint32_t hash_fun_a;
	uint32_t hash_fun_b;
	uint64_t hash_fun_p;

	/* number of elements inserted */
	uint64_t count;

	/* set_buckets */
	TOID(struct set_buckets) buckets;
};

#endif /* SET_NOTX_H */
