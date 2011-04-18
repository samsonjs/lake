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

#include <stdlib.h>

#define LAKE_VERSION "0.1"

typedef int LakeType;

#define TYPE_NIL  0
#define TYPE_SYM  1
#define TYPE_BOOL 2
#define TYPE_INT  3
#define TYPE_STR  4
#define TYPE_LIST 5

#define VAL(x) ((LakeVal *)x)
#define SYM(x) ((LakeSym *)x)
#define BOOL(x) ((LakeBool *)x)
#define INT(x) ((LakeInt *)x)
#define STR(x) ((LakeStr *)x)
#define LIST(x) ((LakeList *)x)

struct lake_val {
    LakeType type;
    size_t size;
};
typedef struct lake_val LakeVal;

LakeVal *NIL;
typedef LakeVal *NILP;

#define VAL_SIZE(x) (x != NULL ? VAL(x)->size : NIL->size)
#define VAL_OR_NIL(x) (x != NULL ? VAL(x) : NIL)

struct lake_sym {
    LakeVal base;
    size_t n;
    char *s;
    unsigned long hash;
};
typedef struct lake_sym LakeSym;

struct lake_int {
    LakeVal base;
    int val;
};
typedef struct lake_int LakeInt;

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
#define LIST_VAL(list, i) (list->vals[i])

void die(char *msg);
void oom();
char *repr(LakeVal *val);

#include "sym.h"
#include "bool.h"
#include "int.h"
#include "list.h"
#include "string.h"

#endif