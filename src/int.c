/** 
  * int.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include "bool.h"
#include "int.h"

static LakeInt *int_alloc(void)
{
    LakeInt *i = g_malloc(sizeof(LakeInt));
    VAL(i)->type = TYPE_INT;
    VAL(i)->size = sizeof(LakeInt);
    return i;
}

LakeInt *int_make(void)
{
    LakeInt *i = int_alloc();
    i->val = 0;
    return i;
}

LakeInt *int_from_c(int n)
{
    LakeInt *i = int_alloc();
    i->val = n;
    return i;
}

LakeStr *int_to_str(LakeInt *i)
{
    char *s = g_strdup_printf("%d", i->val);
    LakeStr *str = str_from_c(s);
	g_free(s);
    return str;
}
