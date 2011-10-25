/** 
  * str.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "int.h"
#include "lake.h"
#include "str.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static LakeStr *lk_str_alloc(void)
{
  LakeStr *str = malloc(sizeof(LakeStr));
  VAL(str)->type = TYPE_STR;
  VAL(str)->size = sizeof(LakeStr);
  return str;
}

void lk_str_free(LakeStr *str)
{
  free(STR_S(str));
  free(str);
}

static LakeVal *lk_str_set(LakeStr *str, char *s)
{
  STR_N(str) = strlen(s);
  STR_S(str) = strndup(s, STR_N(str));
  return NULL;
}

LakeStr *lk_str_from_c(char *s)
{
  LakeStr *str = lk_str_alloc();
  lk_str_set(str, s);
  return str;
}

LakeStr *lk_str_make(void)
{
  return lk_str_from_c("");
}

LakeInt *lk_str_len(LakeStr *str)
{
  return int_from_c(STR_N(str));
}

LakeStr *lk_str_copy(LakeStr *str)
{
  return lk_str_from_c(STR_S(str));
}

char *lk_str_val(LakeStr *str)
{
  return strndup(STR_S(str), STR_N(str));
}

bool lk_str_equal(LakeStr *a, LakeStr *b)
{
  if (STR_N(a) != STR_N(b)) return FALSE;
  size_t n = MIN(STR_N(a), STR_N(b));
  return strncmp(STR_S(a), STR_S(b), n) == 0;
}

LakeStr *lk_str_to_str(LakeStr *str)
{
  return lk_str_copy(str);
}
