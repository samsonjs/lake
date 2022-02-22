#include "laketest.h"

/**
 * sym.h
 * Lake Scheme
 *
 * Copyright 2011 Sami Samhuri
 * MIT License
 *
 */

#ifndef _LAKE_SYM_H
#define _LAKE_SYM_H

#include "lake.h"

LakeSym *sym_intern(LakeCtx *ctx, char *s);
LakeStr *sym_to_str(LakeSym *sym);
LakeSym *sym_from_str(LakeCtx *ctx, LakeStr *str);
char *sym_repr(LakeSym *sym);
unsigned long sym_val(LakeSym *sym);

#endif