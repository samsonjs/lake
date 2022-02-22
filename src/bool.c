/**
 * bool.c
 * Lake Scheme
 *
 * Copyright 2011 Sami Samhuri
 * MIT License
 *
 */

#include "bool.h"
#include "common.h"
#include "lake.h"
#include <string.h>

bool lake_bool_val(LakeBool *b) { return b->val; }

bool lake_is_true(LakeCtx *ctx, LakeVal *x) { return VAL(x) == VAL(ctx->T); }

bool lake_is_false(LakeCtx *ctx, LakeVal *x) { return VAL(x) == VAL(ctx->F); }

bool lake_is_truthy(LakeCtx *ctx, LakeVal *x) { return !lake_is_false(ctx, x); }

bool lake_is_falsy(LakeCtx *ctx, LakeVal *x) { return lake_is_false(ctx, x); }

LakeBool *lake_bool_from_int(LakeCtx *ctx, int n)
{
    return n ? ctx->T : ctx->F;
}

char *lake_bool_repr(LakeBool *b)
{
    return strdup(lake_bool_val(b) ? "#t" : "#f");
}

LakeVal *lake_bool_and(LakeCtx *ctx, LakeVal *x, LakeVal *y)
{
    return lake_is_truthy(ctx, x) && lake_is_truthy(ctx, y) ? y : x;
}

LakeVal *lake_bool_or(LakeCtx *ctx, LakeVal *x, LakeVal *y)
{
    return lake_is_truthy(ctx, x) ? x : y;
}
