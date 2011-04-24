/** 
  * fn.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_FN_H
#define _LAKE_FN_H 1

#include "env.h"
#include "lake.h"

LakeFn *fn_make(LakeList *params, LakeSym *varargs, LakeList *body, Env *closure);
char *fn_repr(LakeFn *fn);

#endif