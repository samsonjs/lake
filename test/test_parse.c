#include "laketest.h"

/**
  * parse.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_PARSE_H
#define _LAKE_PARSE_H

#include <stdlib.h>
#include "lake.h"

#define PARSE_EOF -1
#define PARSE_ERR -2

LakeVal *parse_expr(LakeCtx *ctx, char *s, size_t n);
LakeList *parse_exprs(LakeCtx *ctx, char *s, size_t n);
LakeList *parse_naked_list(LakeCtx *ctx, char *s, size_t n);

#endif
