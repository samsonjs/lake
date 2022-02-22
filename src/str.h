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

LakeStr *lake_str_make(void);
void lake_str_free(LakeStr *str);
LakeStr *lake_str_copy(LakeStr *str);
LakeStr *lake_str_from_c(char *s);
char *lake_str_val(LakeStr *str);
LakeInt *lake_str_len(LakeStr *str);
bool lake_str_equal(LakeStr *a, LakeStr *b);
LakeStr *lake_str_to_str(LakeStr *str);

#endif