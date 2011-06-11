/** 
  * str.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "int.h"
#include "lake.h"
#include "str.h"

static LakeStr *str_alloc(void)
{
    LakeStr *str = g_malloc(sizeof(LakeStr));
    VAL(str)->type = TYPE_STR;
    VAL(str)->size = sizeof(LakeStr);
    return str;
}

void str_free(LakeStr *str)
{
    g_free(str->s);
    g_free(str);
}

static LakeVal *str_set(LakeStr *str, char *s)
{
    str->n = strlen(s);
    str->s = g_strdup(s);
    return NULL;
}

LakeStr *str_from_c(char *s)
{
    LakeStr *str = str_alloc();
    str_set(str, s);
    return str;
}

LakeStr *str_make(void)
{
    return str_from_c("");
}

LakeInt *str_len(LakeStr *str)
{
    return int_from_c(str->n);
}

LakeStr *str_copy(LakeStr *str)
{
    return str_from_c(str->s);
}

char *str_val(LakeStr *str)
{
    return g_strdup(str->s);
}

bool str_equal(LakeStr *a, LakeStr *b)
{
    size_t n = STR_N(a);
    if (n != STR_N(b)) return FALSE;
    return g_strcmp0(a->s, b->s) == 0;
}

LakeStr *str_to_str(LakeStr *str)
{
    return str_copy(str);
}
