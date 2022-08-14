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

/*
 * map_hashmap_notx.c -- common interface for maps
 */

#include <map.h>
#include <hashmap_notx.h>

#include "map_hashmap_notx.h"

/*
 * map_hm_notx_check -- wrapper for hm_notx_check
 */
static int
map_hm_notx_check(PMEMobjpool *pop, TOID(struct map) map)
{
	TOID(struct hashmap_notx) hashmap_notx;
	TOID_ASSIGN(hashmap_notx, map.oid);

	return hm_notx_check(pop, hashmap_notx);
}

/*
 * map_hm_notx_count -- wrapper for hm_notx_count
 */
static size_t
map_hm_notx_count(PMEMobjpool *pop, TOID(struct map) map)
{
	TOID(struct hashmap_notx) hashmap_notx;
	TOID_ASSIGN(hashmap_notx, map.oid);

	return hm_notx_count(pop, hashmap_notx);
}

/*
 * map_hm_notx_init -- wrapper for hm_notx_init
 */
static int
map_hm_notx_init(PMEMobjpool *pop, TOID(struct map) map)
{
	TOID(struct hashmap_notx) hashmap_notx;
	TOID_ASSIGN(hashmap_notx, map.oid);

	return hm_notx_init(pop, hashmap_notx);
}

/*
 * map_hm_notx_create -- wrapper for hm_notx_create
 */
static int
map_hm_notx_create(PMEMobjpool *pop, TOID(struct map) *map, void *arg)
{
	TOID(struct hashmap_notx) *hashmap_notx =
		(TOID(struct hashmap_notx) *)map;

	return hm_notx_create(pop, hashmap_notx, arg);
}

/*
 * map_hm_notx_insert -- wrapper for hm_notx_insert
 */
static int
map_hm_notx_insert(PMEMobjpool *pop, TOID(struct map) map,
		uint64_t key, PMEMoid value)
{
	TOID(struct hashmap_notx) hashmap_notx;
	TOID_ASSIGN(hashmap_notx, map.oid);

	return hm_notx_insert(pop, hashmap_notx, key, value);
}

/*
 * map_hm_notx_remove -- wrapper for hm_notx_remove
 */
static PMEMoid
map_hm_notx_remove(PMEMobjpool *pop, TOID(struct map) map, uint64_t key)
{
	TOID(struct hashmap_notx) hashmap_notx;
	TOID_ASSIGN(hashmap_notx, map.oid);

	return hm_notx_remove(pop, hashmap_notx, key);
}

/*
 * map_hm_notx_get -- wrapper for hm_notx_get
 */
static PMEMoid
map_hm_notx_get(PMEMobjpool *pop, TOID(struct map) map, uint64_t key)
{
	TOID(struct hashmap_notx) hashmap_notx;
	TOID_ASSIGN(hashmap_notx, map.oid);

	return hm_notx_get(pop, hashmap_notx, key);
}

/*
 * map_hm_notx_lookup -- wrapper for hm_notx_lookup
 */
static int
map_hm_notx_lookup(PMEMobjpool *pop, TOID(struct map) map, uint64_t key)
{
	TOID(struct hashmap_notx) hashmap_notx;
	TOID_ASSIGN(hashmap_notx, map.oid);

	return hm_notx_lookup(pop, hashmap_notx, key);
}

/*
 * map_hm_notx_foreach -- wrapper for hm_notx_foreach
 */
static int
map_hm_notx_foreach(PMEMobjpool *pop, TOID(struct map) map,
		int (*cb)(uint64_t key, PMEMoid value, void *arg),
		void *arg)
{
	TOID(struct hashmap_notx) hashmap_notx;
	TOID_ASSIGN(hashmap_notx, map.oid);

	return hm_notx_foreach(pop, hashmap_notx, cb, arg);
}

/*
 * map_hm_notx_cmd -- wrapper for hm_notx_cmd
 */
static int
map_hm_notx_cmd(PMEMobjpool *pop, TOID(struct map) map,
		unsigned cmd, uint64_t arg)
{
	TOID(struct hashmap_notx) hashmap_notx;
	TOID_ASSIGN(hashmap_notx, map.oid);

	return hm_notx_cmd(pop, hashmap_notx, cmd, arg);
}

struct map_ops hashmap_notx_ops = {
	/* .check	= */ map_hm_notx_check,
	/* .create	= */ map_hm_notx_create,
	/* .delete	= */ NULL,
	/* .init	= */ map_hm_notx_init,
	/* .insert	= */ map_hm_notx_insert,
	/* .insert_new	= */ NULL,
	/* .remove	= */ map_hm_notx_remove,
	/* .remove_free	= */ NULL,
	/* .clear	= */ NULL,
	/* .get		= */ map_hm_notx_get,
	/* .lookup	= */ map_hm_notx_lookup,
	/* .foreach	= */ map_hm_notx_foreach,
	/* .is_empty	= */ NULL,
	/* .count	= */ map_hm_notx_count,
	/* .cmd		= */ map_hm_notx_cmd,
};
