/** 
  * lake.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_LAKE_H
#define _LAKE_LAKE_H 1

#include <glib.h>
#include <stdlib.h>

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

#define VAL(x) ((LakeVal *)x)
#define SYM(x) ((LakeSym *)x)
#define BOOL(x) ((LakeBool *)x)
#define INT(x) ((LakeInt *)x)
#define STR(x) ((LakeStr *)x)
#define LIST(x) ((LakeList *)x)
#define DLIST(x) ((LakeDottedList *)x)
#define PRIM(x) ((LakePrimitive *)x)
#define FN(x) ((LakeFn *)x)

struct lake_val {
    LakeType type;
    size_t size;
};
typedef struct lake_val LakeVal;

#define VAL_SIZE(x) (VAL(x)->size)
#define IS(t, x) (VAL(x)->type == t)

struct lake_sym {
    LakeVal base;
    size_t n;
    char *s;
    unsigned long hash;
};
typedef struct lake_sym LakeSym;

#define SYM_S(x) (x->s)
#define SYM_HASH(x) (x->hash)

struct lake_bool {
	LakeVal base;
	gboolean val;
};
typedef struct lake_bool LakeBool;

LakeBool *T;
LakeBool *F;

#define BOOL_VAL(x) (x->val)
#define IS_TRUE(x) (VAL(x) == VAL(T))
#define IS_FALSE(x) (VAL(x) == VAL(F))
#define IS_TRUTHY(x) (!IS_FALSE(x))
#define IS_FALSY(x) (IS_FALSE(x))

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

#define DLIST_HEAD(x) (x->head)
#define DLIST_TAIL(x) (x->tail)

typedef LakeVal *(*lake_fn)(LakeList *args);

struct lake_primitive {
	LakeVal base;
	char *name;
    int arity;
	lake_fn fn;
};
typedef struct lake_primitive LakePrimitive;

#define PRIM_ARITY(x) (x->arity)
#define ARITY_VARARGS -1

#include "env.h"

struct lake_fn {
	LakeVal base;
	LakeList *params;
	LakeSym *varargs;
	LakeList *body;
	Env *closure;
};
typedef struct lake_fn LakeFn;

char *repr(LakeVal *val);

#include <stdio.h>

#define ERR(...) printf("error: "); printf(__VA_ARGS__); putchar('\n')
#define DIE(...) ERR(__VA_ARGS__); exit(1)
#define OOM() DIE("out of memory")

#include "sym.h"
#include "bool.h"
#include "int.h"
#include "string.h"
#include "list.h"
#include "dlist.h"
#include "fn.h"

#include "bootstrap.h"

#endif