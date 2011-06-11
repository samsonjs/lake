/** 
  * int.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include "common.h"
#include "int.h"
#include "lake.h"
#include "str.h"

static LakeInt *int_alloc(void)
{
    LakeInt *i = g_malloc(sizeof(LakeInt));
    VAL(i)->type = TYPE_INT;
    VAL(i)->size = sizeof(LakeInt);
    return i;
}

LakeInt *int_make(void)
{
    return int_from_c(0);
}

LakeInt *int_from_c(int n)
{
    LakeInt *i = int_alloc();
    i->val = n;
    return i;
}

char *int_repr(LakeInt *i)
{
    return g_strdup_printf("%d", i->val);
}

LakeStr *int_to_str(LakeInt *i)
{
    char *s = int_repr(i);
    LakeStr *str = str_from_c(s);
	g_free(s);
    return str;
}
