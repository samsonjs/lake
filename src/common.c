/** 
  * common.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *lake_str_append(char *s1, char *s2)
{
  size_t n2 = strlen(s2);
  s1 = realloc(s1, strlen(s1) + n2 + 1);
  strncat(s1, s2, n2);
  return s1;
}
