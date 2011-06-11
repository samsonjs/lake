/** 
  * eval.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_EVAL_H
#define _LAKE_EVAL_H 1

#include "env.h"
#include "lake.h"

LakeVal *eval_str(LakeCtx *ctx, Env *env, char *s);
LakeVal *eval(LakeCtx *ctx, Env *env, LakeVal *expr);
LakeList *eval_exprs(LakeCtx *ctx, Env *env, LakeList *exprs);
LakeVal *eval_exprs1(LakeCtx *ctx, Env *env, LakeList *exprs);
LakeVal *apply(LakeCtx *ctx, LakeVal *fnVal, LakeList *args);
bool is_special_form(LakeCtx *ctx, LakeList *expr);
void init_special_form_handlers(LakeCtx *ctx);

#endif
