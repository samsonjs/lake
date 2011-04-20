/** 
  * parse.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_PARSE_H
#define _LAKE_PARSE_H 1

#include <stdlib.h>
#include "lake.h"

#define PARSE_EOF -1
#define PARSE_ERR -2

LakeVal *parse_expr(char *s, size_t n);

#endif