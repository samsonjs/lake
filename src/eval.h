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

LakeVal *eval(Env *env, LakeVal *expr);
LakeList *eval_exprs(Env *env, LakeList *exprs);
LakeVal *apply(LakeVal *fnVal, LakeList *args);
gboolean is_special_form(LakeList *expr);

#endif