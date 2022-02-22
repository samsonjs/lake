/**
 * bool.h
 * Lake Scheme
 *
 * Copyright 2011 Sami Samhuri
 * MIT License
 *
 */

#ifndef _LAKE_BOOL_H
#define _LAKE_BOOL_H

#include "common.h"
#include "lake.h"

bool lake_bool_val(LakeBool *b);
bool lake_is_true(LakeCtx *ctx, LakeVal *x);
bool lake_is_false(LakeCtx *ctx, LakeVal *x);
bool lake_is_truthy(LakeCtx *ctx, LakeVal *x);
bool lake_is_falsy(LakeCtx *ctx, LakeVal *x);
LakeBool *lake_bool_from_int(LakeCtx *ctx, int n);
char *lake_bool_repr(LakeBool *b);
LakeVal *lake_bool_and(LakeCtx *ctx, LakeVal *x, LakeVal *y);
LakeVal *lake_bool_or(LakeCtx *ctx, LakeVal *x, LakeVal *y);

#endif
