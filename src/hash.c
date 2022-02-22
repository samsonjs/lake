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

void lake_hash_put(khash_t(LakeVal) * h, char *key, void *val)
{
    int ret;
    khiter_t k = kh_put(LakeVal, h, key, &ret);
    kh_value(h, k) = val;
}

void *lake_hash_get(khash_t(LakeVal) * h, char *key)
{
    khiter_t k = kh_get(LakeVal, h, key);
    return k == kh_end(h) ? NULL : kh_value(h, k);
}

bool lake_hash_has(khash_t(LakeVal) * h, char *key)
{
    khiter_t k = kh_get(LakeVal, h, key);
    return k != kh_end(h) && kh_exist(h, k);
}
