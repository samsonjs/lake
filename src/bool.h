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

#include <glib.h>
#include "lake.h"

LakeBool *bool_from_int(int b);
gboolean bool_val(LakeBool *b);
LakeStr *bool_to_str(LakeBool *b);
char *bool_repr(LakeBool *b);
LakeVal* bool_and(LakeVal *a, LakeVal *b);
LakeVal* bool_or(LakeVal *a, LakeVal *b);

#endif