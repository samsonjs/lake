/** 
  * int.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_INT_H
#define _LAKE_INT_H

#include "lake.h"

/* Min value of 64-bit int is −9,223,372,036,854,775,808 */
#define MAX_INT_LENGTH 20

LakeInt *int_make(void);
LakeInt *int_from_c(int n);
LakeStr *int_to_str(LakeInt *i);
char *int_repr(LakeInt *i);

#endif