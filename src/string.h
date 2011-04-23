/** 
  * string.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_STRING_H
#define _LAKE_STRING_H 1

#include <glib.h>
#include "lake.h"

LakeStr *str_make(void);
void str_free(LakeStr *str);
LakeStr *str_copy(LakeStr *str);
LakeStr *str_from_c(char *s);
char *str_val(LakeStr *str);
LakeInt *str_len(LakeStr *str);
gboolean str_equal(LakeStr *a, LakeStr *b);
LakeStr *str_to_str(LakeStr *str);

#endif