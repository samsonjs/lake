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
LakeVal *apply(LakeVal *fnVal, LakeList *args);

#endif