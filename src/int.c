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

LakeInt *int_copy(LakeInt *i)
{
    LakeInt *copy = int_alloc();
    copy->val = i->val;
    return copy;
}

LakeInt *int_from_c(int n)
{
    LakeInt *i = int_alloc();
    i->val = n;
    return i;
}

LakeInt *int_cmp(LakeInt *a, LakeInt *b)
{
    int aN = a->val, bN = b->val;
    LakeInt *result = int_alloc();
    result->val = aN < bN ? -1 : (aN == bN ? 0 : 1);
    return result;
}

LakeBool *int_eq(LakeInt *a, LakeInt *b)
{
    return bool_from_int(a->val == b->val);
}

LakeStr *int_to_str(LakeInt *i)
{
    char *s = g_strdup_printf("%d", i->val);
    LakeStr *str = str_from_c(s);
	g_free(s);
    return str;
}
