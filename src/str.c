/**
 * str.c
 * Lake Scheme
 *
 * Copyright 2011 Sami Samhuri
 * MIT License
 *
 */

#include "str.h"
#include "common.h"
#include "int.h"
#include "lake.h"
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static LakeStr *lake_str_alloc(void)
{
    LakeStr *str = malloc(sizeof(LakeStr));
    VAL(str)->type = TYPE_STR;
    VAL(str)->size = sizeof(LakeStr);
    return str;
}

void lake_str_free(LakeStr *str)
{
    free(STR_S(str));
    free(str);
}

static LakeVal *lake_str_set(LakeStr *str, char *s)
{
    STR_N(str) = strlen(s);
    STR_S(str) = strndup(s, STR_N(str));
    return NULL;
}

LakeStr *lake_str_from_c(char *s)
{
    LakeStr *str = lake_str_alloc();
    lake_str_set(str, s);
    return str;
}

LakeStr *lake_str_make(void) { return lake_str_from_c(""); }

LakeInt *lake_str_len(LakeStr *str) { return int_from_c(STR_N(str)); }

LakeStr *lake_str_copy(LakeStr *str) { return lake_str_from_c(STR_S(str)); }

char *lake_str_val(LakeStr *str) { return strndup(STR_S(str), STR_N(str)); }

bool lake_str_equal(LakeStr *a, LakeStr *b)
{
    if (STR_N(a) != STR_N(b)) return FALSE;
    size_t n = MIN(STR_N(a), STR_N(b));
    return strncmp(STR_S(a), STR_S(b), n) == 0;
}

LakeStr *lake_str_to_str(LakeStr *str) { return lake_str_copy(str); }
