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
LakeInt *int_from_c(int n);
LakeStr *int_to_str(LakeInt *i);
char *int_repr(LakeInt *i);

#endif