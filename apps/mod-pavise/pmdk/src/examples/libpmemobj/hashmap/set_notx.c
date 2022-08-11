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

/* integer hash set implementation which uses only transaction APIs */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <inttypes.h>

#include <libpmemobj.h>
#include "set_notx.h"
#include "hashmap_internal.h"

/*
 * create_set -- set initializer
 */
static void
create_set(PMEMobjpool *pop, TOID(struct set_notx) set, uint32_t seed)
{
	size_t len = INIT_BUCKETS_NUM;
	size_t sz = sizeof(struct set_buckets) +
			len * sizeof(TOID(struct set_entry));
/*
	TX_BEGIN(pop) {
		TX_ADD(set);
*/
		D_RW(set)->seed = seed;
		do {
			D_RW(set)->hash_fun_a = (uint32_t)rand();
		} while (D_RW(set)->hash_fun_a == 0);
		D_RW(set)->hash_fun_b = (uint32_t)rand();
		D_RW(set)->hash_fun_p = HASH_FUNC_COEFF_P;
        if (POBJ_ZALLOC(pop, &D_RW(set)->buckets, struct set_buckets, sz)) {
            fprintf(stderr, "root alloc failed: %s\n", pmemobj_errormsg());
            abort();
        }
		D_RW(D_RW(set)->buckets)->nbuckets = len;
/*	} TX_ONABORT {
		fprintf(stderr, "%s: transaction aborted: %s\n", __func__,
			pmemobj_errormsg());
		abort();
	} TX_END
*/
}

/*
 * hash -- the simplest hashing function,
 * see https://en.wikipedia.org/wiki/Universal_hashing#Hashing_integers
 */
static uint64_t
hash(const TOID(struct set_notx) *set,
	const TOID(struct set_buckets) *buckets, uint64_t value)
{
	uint32_t a = D_RO(*set)->hash_fun_a;
	uint32_t b = D_RO(*set)->hash_fun_b;
	uint64_t p = D_RO(*set)->hash_fun_p;
	size_t len = D_RO(*buckets)->nbuckets;

	return ((a * value + b) % p) % len;
}

/*
 * set_notx_rebuild -- rebuilds the set with a new number of buckets
 */
static void
set_notx_rebuild(PMEMobjpool *pop, TOID(struct set_notx) set, size_t new_len)
{
	TOID(struct set_buckets) buckets_old = D_RO(set)->buckets;

	if (new_len == 0)
		new_len = D_RO(buckets_old)->nbuckets;
/*
	size_t sz_old = sizeof(struct set_buckets) +
			D_RO(buckets_old)->nbuckets *
			sizeof(TOID(struct set_entry));
            */
	size_t sz_new = sizeof(struct set_buckets) +
			new_len * sizeof(TOID(struct set_entry));

    /*
	TX_BEGIN(pop) {
		TX_ADD_FIELD(set, buckets);
        */
        TOID(struct set_buckets) buckets_new;
        POBJ_ZALLOC(pop, &buckets_new, struct set_buckets, sz_new);
		D_RW(buckets_new)->nbuckets = new_len;
//		pmemobj_tx_add_range(buckets_old.oid, 0, sz_old);

		for (size_t i = 0; i < D_RO(buckets_old)->nbuckets; ++i) {
			while (!TOID_IS_NULL(D_RO(buckets_old)->bucket[i])) {
				TOID(struct set_entry) en =
					D_RO(buckets_old)->bucket[i];
				uint64_t h = hash(&set, &buckets_new,
						D_RO(en)->key);

				D_RW(buckets_old)->bucket[i] = D_RO(en)->next;

				//TX_ADD_FIELD(en, next);
				D_RW(en)->next = D_RO(buckets_new)->bucket[h];
				D_RW(buckets_new)->bucket[h] = en;
			}
		}

		D_RW(set)->buckets = buckets_new;
        /*
		TX_FREE(buckets_old);
	} TX_ONABORT {
		fprintf(stderr, "%s: transaction aborted: %s\n", __func__,
			pmemobj_errormsg());
		//
		 * We don't need to do anything here, because everything is
		 * consistent. The only thing affected is performance.
		 //
	} TX_END
    */
}

/*
 * set_notx_insert -- inserts specified value into the set,
 * returns:
 * - 0 if successful,
 * - 1 if value already existed,
 * - -1 if something bad happened
 */
int
set_notx_insert(PMEMobjpool *pop, TOID(struct set_notx) set,
	uint64_t key)
{
	TOID(struct set_buckets) buckets = D_RO(set)->buckets;
	TOID(struct set_entry) var;

	uint64_t h = hash(&set, &buckets, key);
	int num = 0;

	for (var = D_RO(buckets)->bucket[h];
			!TOID_IS_NULL(var);
			var = D_RO(var)->next) {
		if (D_RO(var)->key == key) {
			return 1;
		}
		num++;
	}

	int ret = 0;
    /*
	TX_BEGIN(pop) {
		TX_ADD_FIELD(D_RO(set)->buckets, bucket[h]);
		TX_ADD_FIELD(set, count);
    */
		TOID(struct set_entry) e;
		POBJ_ZNEW(pop, &e, struct set_entry);
		D_RW(e)->key = key;
		D_RW(e)->next = D_RO(buckets)->bucket[h];
		D_RW(buckets)->bucket[h] = e;

		D_RW(set)->count++;
		num++;
	/*} TX_ONABORT {
		fprintf(stderr, "transaction aborted: %s\n",
			pmemobj_errormsg());
		ret = -1;
	} TX_END
    */
	if (ret)
		return ret;

	if (num > MAX_HASHSET_THRESHOLD ||
			(num > MIN_HASHSET_THRESHOLD &&
			D_RO(set)->count > 2 * D_RO(buckets)->nbuckets))
		set_notx_rebuild(pop, set, D_RO(buckets)->nbuckets * 2);

	return 0;
}

/*
 * set_notx_remove -- removes specified element from the set,
 * returns:
 * - 1 if successful,
 * - 0 if value didn't exist or if something bad happened
 */
int
set_notx_remove(PMEMobjpool *pop, TOID(struct set_notx) set, uint64_t key)
{
	TOID(struct set_buckets) buckets = D_RO(set)->buckets;
	TOID(struct set_entry) var, prev = TOID_NULL(struct set_entry);

	uint64_t h = hash(&set, &buckets, key);
	for (var = D_RO(buckets)->bucket[h];
			!TOID_IS_NULL(var);
			prev = var, var = D_RO(var)->next) {
		if (D_RO(var)->key == key)
			break;
	}

	if (TOID_IS_NULL(var))
		return 0;
	int ret = 0;

    /*
	TX_BEGIN(pop) {
		if (TOID_IS_NULL(prev))
			TX_ADD_FIELD(D_RO(set)->buckets, bucket[h]);
		else
			TX_ADD_FIELD(prev, next);
		TX_ADD_FIELD(set, count);
    */

		if (TOID_IS_NULL(prev))
			D_RW(buckets)->bucket[h] = D_RO(var)->next;
		else
			D_RW(prev)->next = D_RO(var)->next;
		D_RW(set)->count--;
    /*
		TX_FREE(var);
	} TX_ONABORT {
		fprintf(stderr, "transaction aborted: %s\n",
			pmemobj_errormsg());
		ret = -1;
	} TX_END
    */
	if (ret)
		return 0;

	if (D_RO(set)->count < D_RO(buckets)->nbuckets)
		set_notx_rebuild(pop, set, D_RO(buckets)->nbuckets / 2);

	return 1; 
}

/*
 * set_notx_foreach -- prints all values from the set
 */
int
set_notx_foreach(PMEMobjpool *pop, TOID(struct set_notx) set,
	int (*cb)(uint64_t key, void *arg), void *arg)
{
	TOID(struct set_buckets) buckets = D_RO(set)->buckets;
	TOID(struct set_entry) var;

	int ret = 0;
	for (size_t i = 0; i < D_RO(buckets)->nbuckets; ++i) {
		if (TOID_IS_NULL(D_RO(buckets)->bucket[i]))
			continue;

		for (var = D_RO(buckets)->bucket[i]; !TOID_IS_NULL(var);
				var = D_RO(var)->next) {
			ret = cb(D_RO(var)->key, arg);
			if (ret)
				break;
		}
	}

	return ret;
}

/*
 * set_notx_debug -- prints complete set state
 */
static void
set_notx_debug(PMEMobjpool *pop, TOID(struct set_notx) set, FILE *out)
{
	TOID(struct set_buckets) buckets = D_RO(set)->buckets;
	TOID(struct set_entry) var;

	fprintf(out, "a: %u b: %u p: %" PRIu64 "\n", D_RO(set)->hash_fun_a,
		D_RO(set)->hash_fun_b, D_RO(set)->hash_fun_p);
	fprintf(out, "count: %" PRIu64 ", buckets: %zu\n",
		D_RO(set)->count, D_RO(buckets)->nbuckets);

	for (size_t i = 0; i < D_RO(buckets)->nbuckets; ++i) {
		if (TOID_IS_NULL(D_RO(buckets)->bucket[i]))
			continue;

		int num = 0;
		fprintf(out, "%zu: ", i);
		for (var = D_RO(buckets)->bucket[i]; !TOID_IS_NULL(var);
				var = D_RO(var)->next) {
			fprintf(out, "%" PRIu64 " ", D_RO(var)->key);
			num++;
		}
		fprintf(out, "(%d)\n", num);
	}
}

/*
 * set_notx_lookup -- checks whether specified element exists
 */
int
set_notx_lookup(PMEMobjpool *pop, TOID(struct set_notx) set, uint64_t key)
{
	TOID(struct set_buckets) buckets = D_RO(set)->buckets;
	TOID(struct set_entry) var;

	uint64_t h = hash(&set, &buckets, key);

	for (var = D_RO(buckets)->bucket[h];
			!TOID_IS_NULL(var);
			var = D_RO(var)->next)
		if (D_RO(var)->key == key)
			return 1;

	return 0;
}

/*
 * set_notx_count -- returns number of elements
 */
size_t
set_notx_count(PMEMobjpool *pop, TOID(struct set_notx) set)
{
	return D_RO(set)->count;
}

/*
 * set_notx_init -- recovers set state, called after pmemobj_open
 */
int
set_notx_init(PMEMobjpool *pop, TOID(struct set_notx) set)
{
	srand(D_RO(set)->seed);
	return 0;
}

/*
 * set_notx_create -- allocates new set
 */
int
set_notx_create(PMEMobjpool *pop, TOID(struct set_notx) *map, void *arg)
{
	struct set_args *args = (struct set_args *)arg;
	int ret = 0;
	/*TX_BEGIN(pop) {
		TX_ADD_DIRECT(map);
        */
        POBJ_ZNEW(pop, map, struct set_notx);

		uint32_t seed = args ? args->seed : 0;
		create_set(pop, *map, seed);
        /*
	} TX_ONABORT {
		ret = -1;
	} TX_END
    */
	return ret;
}

/*
 * set_notx_check -- checks if specified persistent object is an
 * instance of set
 */
int
set_notx_check(PMEMobjpool *pop, TOID(struct set_notx) set)
{
	return TOID_IS_NULL(set) || !TOID_VALID(set);
}

/*
 * set_notx_cmd -- execute cmd for set
 */
int
set_notx_cmd(PMEMobjpool *pop, TOID(struct set_notx) set,
		unsigned cmd, uint64_t arg)
{
	switch (cmd) {
		case SET_CMD_REBUILD:
			set_notx_rebuild(pop, set, arg);
			return 0;
		case SET_CMD_DEBUG:
			if (!arg)
				return -EINVAL;
			set_notx_debug(pop, set, (FILE *)arg);
			return 0;
		default:
			return -EINVAL;
	}
}
