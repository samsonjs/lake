/** 
  * sym.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "env.h"
#include "lake.h"
#include "str.h"
#include "sym.h"

/* djb's hash
 * http://www.cse.yorku.ca/~oz/hash.html
 */
static uint32_t str_hash(const char *s)
{
	char c;
  uint32_t h = 5381;

  while ((c = *s++))
    h = ((h << 5) + h) ^ c;

  return h;
}

static LakeSym *sym_alloc(void)
{
  LakeSym *sym = malloc(sizeof(LakeSym));
  VAL(sym)->type = TYPE_SYM;
  VAL(sym)->size = sizeof(LakeSym);
  return sym;
}

LakeSym *sym_intern(LakeCtx *ctx, char *s)
{
  LakeSym *sym = lk_hash_get(ctx->symbols, s);
  if (!sym) {
    sym = sym_alloc();
    sym->n = strlen(s);
    sym->s = strndup(s, sym->n);
    sym->hash = str_hash(s);
    lk_hash_put(ctx->symbols, sym->s, sym);
  }
  return sym;
}

LakeStr *sym_to_str(LakeSym *sym)
{
  return lk_str_from_c(sym->s);
}

LakeSym *sym_from_str(LakeCtx *ctx, LakeStr *str)
{
  return sym_intern(ctx, str->s);
}

char *sym_repr(LakeSym *sym)
{
  return strndup(sym->s, sym->n);
}

unsigned long sym_val(LakeSym *sym)
{
  return sym->hash;
}
