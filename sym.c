/** 
  * sym.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "env.h"
#include "lake.h"
#include "string.h"
#include "sym.h"

#define SYM_TABLE_SIZE 1024

static LakeSym *sym_alloc(void)
{
    LakeSym *sym = g_malloc(sizeof(LakeSym));
    sym->base.type = TYPE_SYM;
    sym->base.size = sizeof(LakeSym);
    return sym;
}

LakeSym *sym_intern(char *s)
{
    GHashTable *symbols = shared_env()->symbols;
    LakeSym *sym = g_hash_table_lookup(symbols, s);
    if (!sym) {
        sym = sym_alloc();
        sym->n = strlen(s);
        sym->s = g_strdup(s);
        sym->hash = g_str_hash(s);
        g_hash_table_insert(symbols, sym->s, sym);
	}
    return sym;
}

LakeStr *sym_to_str(LakeSym *sym)
{
    return str_from_c(sym->s);
}

LakeSym *sym_from_str(LakeStr *str)
{
    return sym_intern(str->s);
}

char *sym_repr(LakeSym *sym)
{
    return g_strdup(sym->s);
}

unsigned long sym_val(LakeSym *sym)
{
    return sym->hash;
}

LakeSym *sym_eq(LakeSym *a, LakeSym *b)
{
    return bool_from_int(g_strcmp0(a->s, b->s) == 0);
}
