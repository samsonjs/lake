/** 
  * int.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_INT_H
#define _LAKE_INT_H 1

#include "lake.h"

LakeInt *int_make(void);
LakeInt *int_copy(LakeInt *i);
LakeInt *int_from_c(int n);
LakeInt *int_cmp(LakeInt *a, LakeInt *b);
LakeBool *int_eq(LakeInt *a, LakeInt *b);
LakeStr *int_to_str(LakeInt *i);

#endif