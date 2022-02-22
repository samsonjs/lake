/**
 * hash.h
 * Lake Scheme
 *
 * Copyright 2011 Sami Samhuri
 * MIT License
 *
 * Lifted from TJ Holowaychuk's Luna.
 * https://raw.github.com/visionmedia/luna
 *
 */

#ifndef _LAKE_HASH_H
#define _LAKE_HASH_H

#include "common.h"
#include "khash.h"

KHASH_MAP_INIT_STR(value, void *);

typedef khash_t(value) lake_hash_t;

#define lake_hash_make() kh_init(value)
#define lake_hash_free(h) kh_destroy(value, h)

bool lake_hash_has(khash_t(value) * h, char *key);
void lake_hash_put(khash_t(value) * h, char *key, void *val);
void *lake_hash_get(khash_t(value) * h, char *key);

#endif
