/** 
  * str.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_STRING_H
#define _LAKE_STRING_H

#include "common.h"
#include "lake.h"

LakeStr *lk_str_make(void);
void lk_str_free(LakeStr *str);
LakeStr *lk_str_copy(LakeStr *str);
LakeStr *lk_str_from_c(char *s);
char *lk_str_val(LakeStr *str);
LakeInt *lk_str_len(LakeStr *str);
bool lk_str_equal(LakeStr *a, LakeStr *b);
LakeStr *lk_str_to_str(LakeStr *str);

#endif