/** 
  * primitive.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include "lake.h"
#include "primitive.h"

static LakePrimitive *prim_alloc(void)
{
    LakePrimitive *prim = malloc(sizeof(LakePrimitive));
    VAL(prim)->type = TYPE_PRIM;
    VAL(prim)->size = sizeof(LakePrimitive);
    return prim;
}

LakePrimitive *prim_make(char *name, int arity, lake_fn fn)
{
    LakePrimitive *prim = prim_alloc();
    prim->name = g_strdup(name);
    prim->arity = arity;
    prim->fn = fn;
    return prim;
}