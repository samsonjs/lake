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

#include "khash.h"
#include "common.h"

KHASH_MAP_INIT_STR(value, void *);

typedef khash_t(value) lk_hash_t;

#define lk_hash_make() kh_init(value)
#define lk_hash_free(h) kh_destroy(value, h)

bool lk_hash_has(khash_t(value) *h, char *key);
void lk_hash_put(khash_t(value) *h, char *key, void *val);
void *lk_hash_get(khash_t(value) *h, char *key);

#endif
