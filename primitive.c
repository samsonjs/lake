/** 
  * primitive.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include <stdlib.h>
#include "env.h"
#include "lake.h"
#include "primitive.h"

static LakePrimitive *prim_alloc(void)
{
    LakePrimitive *prim = malloc(sizeof(LakePrimitive));
    VAL(prim)->type = TYPE_PRIM;
    VAL(prim)->size = sizeof(LakePrimitive);
    return prim;
}

LakePrimitive *prim_make(char *name, int arity, lake_fn fn)
{
    LakePrimitive *prim = prim_alloc();
    prim->name = g_strdup(name);
    prim->arity = arity;
    prim->fn = fn;
    return prim;
}

char *prim_repr(LakePrimitive *prim)
{
    return g_strdup_printf("<#primitive:%s(%d)>", prim->name, prim->arity);
}

static LakeVal *prim_nullP(LakeList *args)
{
    LakeVal *val = list_shift(args);
    LakeBool *is_null = IS(TYPE_LIST, val) && LIST_N(LIST(val)) == 0 ? T : F;
    return VAL(is_null);
}

static LakeVal *prim_pairP(LakeList *args)
{
    LakeVal *val = list_shift(args);
    LakeBool *is_pair = IS(TYPE_LIST, val) && LIST_N(LIST(val)) > 0 ? T : F;
    return VAL(is_pair);
}

static LakeVal *prim_not(LakeList *args)
{
    LakeVal *val = list_shift(args);
    LakeBool *not = IS_FALSE(val) ? T : F;
    return VAL(not);
}

#define ENSURE_INT(x, i) do {                                      \
        if (!IS(TYPE_INT, x)) {                                    \
            ERR("argument %zu is not an integer: %s", i, repr(x)); \
            return NULL;                                           \
        }                                                          \
    } while (0)


static LakeVal *prim_add(LakeList *args)
{
    int result = 0;
    size_t n = LIST_N(args);
    size_t i;
    for (i = 0; i < n; ++i) {
        LakeVal *v = LIST_VAL(args, i);
        ENSURE_INT(v, i);
        result += INT_VAL(INT(v));
    }
    return VAL(int_from_c(result));
}

static LakeVal *prim_sub(LakeList *args)
{
    size_t n = LIST_N(args);
    
    if (n < 1) {
        ERR("- requires at least one argument");
        return NULL;
    }
    
    int result = 0;
    size_t i;
    for (i = 0; i < n; ++i) {
        LakeVal *v = LIST_VAL(args, i);
        ENSURE_INT(v, i);
        result -= INT_VAL(INT(v));
    }
    return VAL(int_from_c(result));
}

static LakeVal *prim_mul(LakeList *args)
{
    int result = 1;
    size_t n = LIST_N(args);
    size_t i;
    for (i = 0; i < n; ++i) {
        LakeVal *v = LIST_VAL(args, i);
        ENSURE_INT(v, i);
        result *= INT_VAL(INT(v));
    }
    return VAL(int_from_c(result));
}

#define DIVIDE_BY_ZERO() ERR("divide by zero")

static LakeVal *prim_div(LakeList *args)
{
    size_t n = LIST_N(args);
    
    if (n < 1) {
        ERR("/ requires at least one argument");
        return NULL;
    }
    
    LakeVal *v = LIST_VAL(args, 0);
    ENSURE_INT(v, (size_t)0);
    int result = INT_VAL(INT(v));

    if (n == 1) {
        if (result == 0) {
            DIVIDE_BY_ZERO();
            return NULL;
        }
        result = 1 / result;
    }
    else {
        size_t i;
        for (i = 1; i < n; ++i) {
            v = LIST_VAL(args, i);
            ENSURE_INT(v, i);
            int val = INT_VAL(INT(v));
            if (val == 0) {
                DIVIDE_BY_ZERO();
                return NULL;
            }
            result /= val;
        }
    }
    return VAL(int_from_c(result));
}

static LakeVal *prim_int_eq(LakeList *args)
{
    gboolean result = TRUE;
    size_t n = LIST_N(args);
    size_t i;
    int curr, prev;
    for (i = 0; i < n; ++i) {
        LakeVal *v = LIST_VAL(args, i);
        ENSURE_INT(v, i);
        curr = INT_VAL(INT(v));
        if (i > 0) {
            result = result && curr == prev;
        }
        prev = INT_VAL(INT(v));
    }
    return VAL(bool_from_int(result));
}

Env *primitive_bindings(void)
{
    #define DEFINE(name, fn, arity) env_define(env, sym_intern(name), VAL(prim_make(name, arity, fn)))
    
    Env *env = env_toplevel();
    DEFINE("null?", prim_nullP, 1);
    DEFINE("pair?", prim_pairP, 1);
    DEFINE("not", prim_not, 1);
    DEFINE("+", prim_add, ARITY_VARARGS);
    DEFINE("-", prim_sub, ARITY_VARARGS);
    DEFINE("*", prim_mul, ARITY_VARARGS);
    DEFINE("/", prim_div, ARITY_VARARGS);
    DEFINE("=", prim_int_eq, ARITY_VARARGS);
    return env;
}
