/** 
  * bool.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <string.h>
#include "bool.h"
#include "common.h"
#include "lake.h"

bool lk_bool_val(LakeBool *b)
{
    return b->val;
}

bool lk_is_true(LakeCtx *ctx, LakeVal *x)
{
    return VAL(x) == VAL(ctx->T);
}

bool lk_is_false(LakeCtx *ctx, LakeVal *x)
{
    return VAL(x) == VAL(ctx->F);
}

bool lk_is_truthy(LakeCtx *ctx, LakeVal *x)
{
    return !lk_is_false(ctx, x);
}

bool lk_is_falsy(LakeCtx *ctx, LakeVal *x)
{
    return lk_is_false(ctx, x);
}

LakeBool *lk_bool_from_int(LakeCtx *ctx, int n)
{
    return n ? ctx->T : ctx->F;
}

char *lk_bool_repr(LakeBool *b)
{
    return strdup(lk_bool_val(b) ? "#t" : "#f");
}

LakeVal *lk_bool_and(LakeCtx *ctx, LakeVal *x, LakeVal *y)
{
    return lk_is_truthy(ctx, x) && lk_is_truthy(ctx, y) ? y : x;
}

LakeVal *lk_bool_or(LakeCtx *ctx, LakeVal *x, LakeVal *y)
{
    return lk_is_truthy(ctx, x) ? x : y;
}
