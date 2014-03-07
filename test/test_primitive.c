#include "laketest.h"

/** 
  * primitive.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_PRIMITIVE_H
#define _LAKE_PRIMITIVE_H

#include "env.h"
#include "lake.h"

LakePrimitive *prim_make(char *name, int arity, lake_prim fn);
char *prim_repr(LakePrimitive *prim);
void bind_primitives(LakeCtx *ctx);

#endif
