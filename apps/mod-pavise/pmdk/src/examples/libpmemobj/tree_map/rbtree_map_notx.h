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
 * rbtree_map.h -- TreeMap sorted collection implementation
 */

#ifndef RBTREE_MAP_NOTX_H
#define RBTREE_MAP_NOTX_H

#include <libpmemobj.h>

#ifndef RBTREE_MAP_TYPE_OFFSET
#define RBTREE_MAP_TYPE_OFFSET 1016
#endif

struct rbtree_map_notx;
TOID_DECLARE(struct rbtree_map_notx, RBTREE_MAP_TYPE_OFFSET + 0);

int rbtree_map_notx_check(PMEMobjpool *pop, TOID(struct rbtree_map_notx) map);
int rbtree_map_notx_create(PMEMobjpool *pop, TOID(struct rbtree_map_notx) *map,
	void *arg);
int rbtree_map_notx_destroy(PMEMobjpool *pop, TOID(struct rbtree_map_notx) *map);
int rbtree_map_notx_insert(PMEMobjpool *pop, TOID(struct rbtree_map_notx) map,
	uint64_t key, PMEMoid value);
int rbtree_map_notx_insert_new(PMEMobjpool *pop, TOID(struct rbtree_map_notx) map,
		uint64_t key, size_t size, unsigned type_num,
		void (*constructor)(PMEMobjpool *pop, void *ptr, void *arg),
		void *arg);
PMEMoid rbtree_map_notx_remove(PMEMobjpool *pop, TOID(struct rbtree_map_notx) map,
		uint64_t key);
int rbtree_map_notx_remove_free(PMEMobjpool *pop, TOID(struct rbtree_map_notx) map,
		uint64_t key);
int rbtree_map_notx_clear(PMEMobjpool *pop, TOID(struct rbtree_map_notx) map);
PMEMoid rbtree_map_notx_get(PMEMobjpool *pop, TOID(struct rbtree_map_notx) map,
		uint64_t key);
int rbtree_map_notx_lookup(PMEMobjpool *pop, TOID(struct rbtree_map_notx) map,
		uint64_t key);
int rbtree_map_notx_foreach(PMEMobjpool *pop, TOID(struct rbtree_map_notx) map,
	int (*cb)(uint64_t key, PMEMoid value, void *arg), void *arg);
int rbtree_map_notx_is_empty(PMEMobjpool *pop, TOID(struct rbtree_map_notx) map);

#endif /* RBTREE_MAP_NOTX_H */
