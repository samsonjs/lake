/** 
  * common.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_COMMON_H
#define _LAKE_COMMON_H

#include <stddef.h>

typedef int bool;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

char *lk_str_append(char *s1, char *s2);

#endif
