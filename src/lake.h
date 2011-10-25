/** 
  * lake.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_LAKE_H
#define _LAKE_LAKE_H

#include <stdlib.h>
#include "common.h"

#define LAKE_VERSION "0.1"

typedef int LakeType;

#define TYPE_SYM   1
#define TYPE_BOOL  2
#define TYPE_INT   3
#define TYPE_STR   4
#define TYPE_LIST  5
#define TYPE_DLIST 6
#define TYPE_PRIM  7
#define TYPE_FN    8
#define TYPE_COMM  9

#define VAL(x) ((LakeVal *)x)
#define SYM(x) ((LakeSym *)x)
#define BOOL(x) ((LakeBool *)x)
#define INT(x) ((LakeInt *)x)
#define STR(x) ((LakeStr *)x)
#define LIST(x) ((LakeList *)x)
#define DLIST(x) ((LakeDottedList *)x)
#define PRIM(x) ((LakePrimitive *)x)
#define FN(x) ((LakeFn *)x)
#define COMM(x) ((LakeComment *)x)

struct lake_val {
  LakeType type;
  size_t size;
};
typedef struct lake_val LakeVal;

struct lake_sym {
  LakeVal base;
  size_t n;
  char *s;
  unsigned long hash;
};
typedef struct lake_sym LakeSym;

struct lake_bool {
  LakeVal base;
  bool val;
};
typedef struct lake_bool LakeBool;

struct lake_int {
  LakeVal base;
  int val;
};
typedef struct lake_int LakeInt;

#define INT_VAL(x) (x->val)

struct lake_str {
  LakeVal base;
  size_t n;
  char *s;
};
typedef struct lake_str LakeStr;

#define STR_N(str) (str->n)
#define STR_S(str) (str->s)

struct lake_list {
  LakeVal base;
  size_t cap;
  size_t n;
  LakeVal **vals;
};
typedef struct lake_list LakeList;

#define LIST_N(list) (list->n)
#define LIST_VALS(list) (list->vals)
#define LIST_VAL(list, i) (i >= 0 && i < list->n ? list->vals[i] : NULL)

struct lake_dlist {
  LakeVal base;
  LakeList *head;
  LakeVal *tail;
};
typedef struct lake_dlist LakeDottedList;

#include "hash.h"
#include "env.h"

/* Execution context */
struct lake_ctx {
  Env *toplevel;
  lk_hash_t *symbols;
  lk_hash_t *special_form_handlers;
  LakeBool *T;
  LakeBool *F;
};
typedef struct lake_ctx LakeCtx;

typedef LakeVal *(*lake_prim)(LakeCtx *ctx, LakeList *args);

struct lake_primitive {
  LakeVal base;
  char *name;
  int arity;
  lake_prim fn;
};
typedef struct lake_primitive LakePrimitive;

#define PRIM_ARITY(x) (x->arity)
#define ARITY_VARARGS -1


struct lake_fn {
  LakeVal base;
  LakeList *params;
  LakeSym *varargs;
  LakeList *body;
  Env *closure;
};
typedef struct lake_fn LakeFn;

#define CALLABLE(x) (lk_is_type(TYPE_FN, x) || lk_is_type(TYPE_PRIM, x))

struct lake_comment {
  LakeVal base;
  LakeStr *text;
};
typedef struct lake_comment LakeComment;

#define COMM_TEXT(x) (x->text)

LakeCtx *lake_init(void);
int lk_val_size(void *x);
int lk_is_type(LakeType t, void *x);
bool lk_is_nil(LakeVal *x);
bool lake_is(LakeVal *a, LakeVal *b);
bool lake_equal(LakeVal *a, LakeVal *b);
char *lake_repr(void *val);

#include <stdio.h>

#define ERR(...) do {                 \
        fprintf(stderr, "error: ");   \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n");        \
    } while (0)

#define DIE(...) do { ERR(__VA_ARGS__); exit(1); } while(0)
#define OOM() DIE("%s:%d out of memory", __FILE__, __LINE__)

#include "bool.h"
#include "sym.h"
#include "int.h"
#include "str.h"
#include "list.h"
#include "dlist.h"
#include "fn.h"
#include "comment.h"
#include "primitive.h"

#endif