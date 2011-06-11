/** 
  * bool.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_BOOL_H
#define _LAKE_BOOL_H 1

#include "common.h"
#include "lake.h"

bool lk_bool_val(LakeBool *b);
bool lk_is_true(LakeCtx *ctx, LakeVal *x);
bool lk_is_false(LakeCtx *ctx, LakeVal *x);
bool lk_is_truthy(LakeCtx *ctx, LakeVal *x);
bool lk_is_falsy(LakeCtx *ctx, LakeVal *x);
LakeBool *lk_bool_from_int(LakeCtx *ctx, int n);
char *lk_bool_repr(LakeBool *b);
LakeVal *lk_bool_and(LakeCtx *ctx, LakeVal *x, LakeVal *y);
LakeVal *lk_bool_or(LakeCtx *ctx, LakeVal *x, LakeVal *y);

#endif
