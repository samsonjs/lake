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

#include "lake.h"

LakeSym *bool_from_int(int b);
int is_true(LakeVal *val);
int is_false(LakeVal *val);
int is_truthy(LakeVal *val);
int is_falsy(LakeVal *val);

#endif