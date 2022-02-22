/**
 * int.c
 * Lake Scheme
 *
 * Copyright 2011 Sami Samhuri
 * MIT License
 *
 */

#include "int.h"
#include "common.h"
#include "lake.h"
#include "str.h"
#include <stdlib.h>

static LakeInt *int_alloc(void)
{
    LakeInt *i = malloc(sizeof(LakeInt));
    VAL(i)->type = TYPE_INT;
    VAL(i)->size = sizeof(LakeInt);
    return i;
}

LakeInt *int_make(void) { return int_from_c(0); }

LakeInt *int_from_c(int n)
{
    LakeInt *i = int_alloc();
    i->val = n;
    return i;
}

char *int_repr(LakeInt *i)
{
    char *s = malloc(MAX_INT_LENGTH + 1);
    snprintf(s, MAX_INT_LENGTH, "%d", i->val);
    return s;
}

LakeStr *int_to_str(LakeInt *i)
{
    char *s = int_repr(i);
    LakeStr *str = lake_str_from_c(s);
    free(s);
    return str;
}
