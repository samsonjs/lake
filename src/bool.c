/** 
  * bool.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include "bool.h"
#include "lake.h"
#include "string.h"

LakeBool *bool_from_int(int n)
{
    return n ? T : F;
}

char *bool_repr(LakeBool *b)
{
	return g_strdup(BOOL_VAL(b) ? "#t" : "#f");
}

LakeStr *bool_to_str(LakeBool *b)
{
	return str_from_c(BOOL_VAL(b) ? "#t" : "#f");
}

LakeVal* bool_and(LakeVal *a, LakeVal *b)
{
    return IS_TRUTHY(a) && IS_TRUTHY(b) ? b : a;
}

LakeVal* bool_or(LakeVal *a, LakeVal *b)
{
    return IS_TRUTHY(a) ? a : b;
}
