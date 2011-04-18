/** 
  * sym.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "env.h"
#include "lake.h"
#include "hashtab.h"
#include "string.h"
#include "sym.h"

#define SYM_TABLE_SIZE 1024

static LakeSym *sym_alloc(void)
{
    LakeSym *sym = malloc(sizeof(LakeSym));
    sym->base.type = TYPE_SYM;
    sym->base.size = sizeof(LakeSym);
    return sym;
}

LakeSym *sym_intern(char *s)
{
    size_t n = strlen(s);
    hashtab_t *symbols = shared_env()->symbols;
    LakeSym *sym = ht_get(symbols, s, n);
    if (!sym) {
        sym = sym_alloc();
        sym->n = n;
        sym->s = strdup(s);
        sym->hash = ht_hash(s, n, symbols->size);
        ht_put(symbols, sym->s, sym->n, sym, VAL_SIZE(sym));
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
    return strdup(sym->s);
}

unsigned long sym_val(LakeSym *sym)
{
    return sym->hash;
}

#define MIN(a, b) (a < b ? a : b)

LakeSym *sym_eq(LakeSym *a, LakeSym *b)
{
    return bool_from_int(strncmp(a->s, b->s, MIN(a->n, b->n)) == 0);
}
