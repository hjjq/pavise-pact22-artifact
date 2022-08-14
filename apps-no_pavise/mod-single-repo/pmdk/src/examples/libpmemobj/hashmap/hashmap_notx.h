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
#ifndef HASHMAP_NOTX_H
#define HASHMAP_NOTX_H

#include <stddef.h>
#include <stdint.h>
#include <hashmap.h>
#include <libpmemobj.h>

#ifndef HASHMAP_NOTX_TYPE_OFFSET
#define HASHMAP_NOTX_TYPE_OFFSET 1008
#endif

struct hashmap_notx;
struct entry_notx;
struct buckets_notx;
TOID_DECLARE(struct hashmap_notx, HASHMAP_NOTX_TYPE_OFFSET + 0);
TOID_DECLARE(struct buckets_notx, HASHMAP_NOTX_TYPE_OFFSET + 1);
TOID_DECLARE(struct entry_notx, HASHMAP_NOTX_TYPE_OFFSET + 2);

int hm_notx_check(PMEMobjpool *pop, TOID(struct hashmap_notx) hashmap);
int hm_notx_create(PMEMobjpool *pop, TOID(struct hashmap_notx) *map, void *arg);
int hm_notx_init(PMEMobjpool *pop, TOID(struct hashmap_notx) hashmap);
int hm_notx_insert(PMEMobjpool *pop, TOID(struct hashmap_notx) hashmap,
		uint64_t key, PMEMoid value);
PMEMoid hm_notx_remove(PMEMobjpool *pop, TOID(struct hashmap_notx) hashmap,
		uint64_t key);
PMEMoid hm_notx_get(PMEMobjpool *pop, TOID(struct hashmap_notx) hashmap,
		uint64_t key);
int hm_notx_lookup(PMEMobjpool *pop, TOID(struct hashmap_notx) hashmap,
		uint64_t key);
int hm_notx_foreach(PMEMobjpool *pop, TOID(struct hashmap_notx) hashmap,
	int (*cb)(uint64_t key, PMEMoid value, void *arg), void *arg);
size_t hm_notx_count(PMEMobjpool *pop, TOID(struct hashmap_notx) hashmap);
int hm_notx_cmd(PMEMobjpool *pop, TOID(struct hashmap_notx) hashmap,
		unsigned cmd, uint64_t arg);

struct entry_notx {
	uint64_t key;
	PMEMoid value;

	/* next entry list pointer */
	TOID(struct entry_notx) next;
};

struct buckets_notx {
	/* number of buckets */
	size_t nbuckets;
	/* array of lists */
	TOID(struct entry_notx) bucket[];
};

struct hashmap_notx {
	/* random number generator seed */
	uint32_t seed;

	/* hash function coefficients */
	uint32_t hash_fun_a;
	uint32_t hash_fun_b;
	uint64_t hash_fun_p;

	/* number of values inserted */
	uint64_t count;

	/* buckets */
	TOID(struct buckets_notx) buckets;
};

#endif /* HASHMAP_NOTX_H */
