/**
 * list.h
 * Lake Scheme
 *
 * Copyright 2011 Sami Samhuri
 * MIT License
 *
 */

#ifndef _LAKE_LIST_H
#define _LAKE_LIST_H

#include "common.h"
#include "lake.h"
#include "str.h"
#include <stdlib.h>

LakeList *list_make(void);
LakeList *list_cons(LakeVal *car, LakeVal *cdr);
void list_free(LakeList *list);
LakeList *list_make_with_capacity(size_t cap);
LakeList *list_from_array(size_t n, LakeVal *vals[]);
LakeList *list_copy(LakeList *list);
LakeVal *list_set(LakeList *list, size_t i, LakeVal *val);
LakeVal *list_append(LakeList *list, LakeVal *val);
LakeVal *list_get(LakeList *list, LakeInt *li);
LakeInt *list_len(LakeList *list);
LakeVal *list_pop(LakeList *list);
LakeVal *list_shift(LakeList *list);
LakeVal *list_unshift(LakeList *list, LakeVal *val);
bool list_equal(LakeList *a, LakeList *b);
LakeStr *list_to_str(LakeList *list);
char *list_repr(LakeList *list);

#endif
