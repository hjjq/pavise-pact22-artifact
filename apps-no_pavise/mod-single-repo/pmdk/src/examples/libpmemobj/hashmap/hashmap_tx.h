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
#ifndef HASHMAP_TX_H
#define HASHMAP_TX_H

#include <stddef.h>
#include <stdint.h>
#include <hashmap.h>
#include <libpmemobj.h>

#ifndef HASHMAP_TX_TYPE_OFFSET
#define HASHMAP_TX_TYPE_OFFSET 1004
#endif

struct hashmap_tx;
struct entry;
struct buckets;

TOID_DECLARE(struct hashmap_tx, HASHMAP_TX_TYPE_OFFSET + 0);
TOID_DECLARE(struct buckets, HASHMAP_TX_TYPE_OFFSET + 1);
TOID_DECLARE(struct entry, HASHMAP_TX_TYPE_OFFSET + 2);

int hm_tx_check(PMEMobjpool *pop, TOID(struct hashmap_tx) hashmap);
int hm_tx_create(PMEMobjpool *pop, TOID(struct hashmap_tx) *map, void *arg);
int hm_tx_init(PMEMobjpool *pop, TOID(struct hashmap_tx) hashmap);
int hm_tx_insert(PMEMobjpool *pop, TOID(struct hashmap_tx) hashmap,
		uint64_t key, PMEMoid value);
PMEMoid hm_tx_remove(PMEMobjpool *pop, TOID(struct hashmap_tx) hashmap,
		uint64_t key);
PMEMoid hm_tx_get(PMEMobjpool *pop, TOID(struct hashmap_tx) hashmap,
		uint64_t key);
int hm_tx_lookup(PMEMobjpool *pop, TOID(struct hashmap_tx) hashmap,
		uint64_t key);
int hm_tx_foreach(PMEMobjpool *pop, TOID(struct hashmap_tx) hashmap,
	int (*cb)(uint64_t key, PMEMoid value, void *arg), void *arg);
size_t hm_tx_count(PMEMobjpool *pop, TOID(struct hashmap_tx) hashmap);
int hm_tx_cmd(PMEMobjpool *pop, TOID(struct hashmap_tx) hashmap,
		unsigned cmd, uint64_t arg);

struct entry {
	uint64_t key;
	PMEMoid value;

	/* next entry list pointer */
	TOID(struct entry) next;
};

struct buckets {
	/* number of buckets */
	size_t nbuckets;
	/* array of lists */
	TOID(struct entry) bucket[];
};

struct hashmap_tx {
	/* random number generator seed */
	uint32_t seed;

	/* hash function coefficients */
	uint32_t hash_fun_a;
	uint32_t hash_fun_b;
	uint64_t hash_fun_p;

	/* number of values inserted */
	uint64_t count;

	/* buckets */
	TOID(struct buckets) buckets;
};


#endif /* HASHMAP_TX_H */
