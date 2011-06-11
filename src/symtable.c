/** 
  * symtable.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include "common.h"
#include "lake.h"
#include "symtable.h"

static guint _sym_hash(gconstpointer key)
{
    return sym_val(SYM(key));
}

static bool _sym_eq(gconstpointer a, gconstpointer b)
{
    return a == b;
}

GHashTable *symtable_make(void)
{
    return g_hash_table_new(_sym_hash, _sym_eq);
}
