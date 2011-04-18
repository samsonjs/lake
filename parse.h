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

LakeVal *parse_expr(char *s, size_t n);

#endif