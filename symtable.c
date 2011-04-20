/** 
  * symtable.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include "lake.h"
#include "symtable.h"

static guint _sym_hash(gconstpointer key)
{
    return SYM_HASH(SYM(key));
}

static gboolean _sym_eq(gconstpointer a, gconstpointer b)
{
    return BOOL_VAL(sym_eq(SYM(a), SYM(b)));
}

GHashTable *symtable_make(void)
{
    return g_hash_table_new(_sym_hash, _sym_eq);
}
