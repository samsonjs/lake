/** 
  * symtable.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_SYMTABLE_H
#define _LAKE_SYMTABLE_H 1

#include <glib.h>
#include "common.h"

GHashTable *symtable_make(void);

#endif