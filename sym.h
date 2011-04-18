/** 
  * sym.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_SYM_H
#define _LAKE_SYM_H 1

#include "lake.h"

LakeSym *sym_intern(char *s);
LakeStr *sym_to_str(LakeSym *sym);
LakeSym *sym_from_str(LakeStr *str);
char *sym_repr(LakeSym *sym);
unsigned long sym_val(LakeSym *sym);
LakeSym *sym_eq(LakeSym *a, LakeSym *b);

#endif