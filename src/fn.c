/** 
  * fn.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <stdlib.h>
#include "common.h"
#include "env.h"
#include "fn.h"
#include "lake.h"

static LakeFn *fn_alloc(void)
{
  LakeFn *fn = malloc(sizeof(LakeFn));
  VAL(fn)->type = TYPE_FN;
  VAL(fn)->size = sizeof(LakeFn);
  return fn;
}

LakeFn *fn_make(LakeList *params, LakeSym *varargs, LakeList *body, Env *closure)
{
  LakeFn *fn = fn_alloc();
  fn->params = params;
  fn->varargs = varargs;
  fn->body = body;
  fn->closure = closure;
  return fn;
}

char *fn_repr(LakeFn *fn)
{
  char *s = malloc(8);
  s[0] = '\0';
  s = lake_str_append(s, "(lambda ");
  char *s2;
  if (LIST_N(fn->params) && fn->varargs) {
    LakeDottedList *params = dlist_make(fn->params, VAL(fn->varargs));
    s2 = dlist_repr(params);
    s = lake_str_append(s, s2);
    free(s2);
  }
  else if (fn->varargs) {
    s2 = lake_repr(fn->varargs);
    s = lake_str_append(s, s2);
    free(s2);
  }
  else {
    s2 = lake_repr(fn->params);
    s = lake_str_append(s, s2);
    free(s2);
  }
  s = lake_str_append(s, " ");
  int i;
  for (i = 0; i < LIST_N(fn->body); ++i) {
    s2 = lake_repr(LIST_VAL(fn->body, i));
    s = lake_str_append(s, s2);
    free(s2);
    if (i != LIST_N(fn->body) - 1) s = lake_str_append(s, " ");
  }
  return lake_str_append(s, ")");
}
