/** 
  * hash.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  * Lifted from TJ Holowaychuk's Luna.
  * https://raw.github.com/visionmedia/luna
  *
  */

#include "hash.h"

void lk_hash_put(khash_t(value) *h, char *key, void *val) {
  int ret;
  khiter_t k = kh_put(value, h, key, &ret);
  kh_value(h, k) = val;
}

void *lk_hash_get(khash_t(value) *h, char *key) {
  khiter_t k = kh_get(value, h, key);
  return k == kh_end(h) ? NULL : kh_value(h, k);
}

bool lk_hash_has(khash_t(value) *h, char *key) {
  khiter_t k = kh_get(value, h, key);
  return kh_exist(h, k);
}
