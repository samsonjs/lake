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
#include "str.h"
#include "sym.h"

static LakeSym *sym_alloc(void)
{
    LakeSym *sym = g_malloc(sizeof(LakeSym));
    VAL(sym)->type = TYPE_SYM;
    VAL(sym)->size = sizeof(LakeSym);
    return sym;
}

LakeSym *sym_intern(LakeCtx *ctx, char *s)
{
    LakeSym *sym = g_hash_table_lookup(ctx->symbols, s);
    if (!sym) {
        sym = sym_alloc();
        sym->n = strlen(s);
        sym->s = g_strdup(s);
        sym->hash = g_str_hash(s);
        g_hash_table_insert(ctx->symbols, sym->s, sym);
	}
    return sym;
}

LakeStr *sym_to_str(LakeSym *sym)
{
    return str_from_c(sym->s);
}

LakeSym *sym_from_str(LakeCtx *ctx, LakeStr *str)
{
    return sym_intern(ctx, str->s);
}

char *sym_repr(LakeSym *sym)
{
    return g_strdup(sym->s);
}

unsigned long sym_val(LakeSym *sym)
{
    return sym->hash;
}
