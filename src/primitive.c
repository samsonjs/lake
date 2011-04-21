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
#include "comment.h"
#include "env.h"
#include "int.h"
#include "dlist.h"
#include "fn.h"
#include "list.h"
#include "lake.h"
#include "primitive.h"
#include "string.h"

static LakePrimitive *prim_alloc(void)
{
    LakePrimitive *prim = malloc(sizeof(LakePrimitive));
    VAL(prim)->type = TYPE_PRIM;
    VAL(prim)->size = sizeof(LakePrimitive);
    return prim;
}

LakePrimitive *prim_make(char *name, int arity, lake_prim fn)
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

static LakeVal *_car(LakeList *args)
{
    LakeList *list = LIST(LIST_VAL(args, 0));
    if (IS(TYPE_LIST, list) && LIST_N(list) > 0) {
        return LIST_VAL(list, 0);
    }
    ERR("not a pair: %s", list_repr(list));
    return NULL;
}

static LakeVal *_cdr(LakeList *args)
{
    LakeList *list = LIST(LIST_VAL(args, 0));
    if (IS(TYPE_LIST, list) && LIST_N(list) > 0) {
        LakeList *cdr = list_copy(list);
        list_shift(cdr);
        return VAL(cdr);
    }
    ERR("not a pair: %s", list_repr(list));
    return NULL;
}

static LakeVal *_cons(LakeList *args)
{
    LakeVal *car = LIST_VAL(args, 0);
    LakeVal *cdr = LIST_VAL(args, 1);
    return VAL(list_cons(car, cdr));
}

static LakeVal *_nullP(LakeList *args)
{
    LakeVal *val = list_shift(args);
    LakeBool *is_null = IS(TYPE_LIST, val) && LIST_N(LIST(val)) == 0 ? T : F;
    return VAL(is_null);
}

static LakeVal *_pairP(LakeList *args)
{
    LakeVal *val = list_shift(args);
    LakeBool *is_pair = IS(TYPE_LIST, val) && LIST_N(LIST(val)) > 0 ? T : F;
    return VAL(is_pair);
}

static LakeVal *_isP(LakeList *args)
{
    LakeVal *a = LIST_VAL(args, 0);
    LakeVal *b = LIST_VAL(args, 1);
    return VAL(bool_from_int(lake_is(a, b)));
}

static LakeVal *_equalP(LakeList *args)
{
    LakeVal *a = LIST_VAL(args, 0);
    LakeVal *b = LIST_VAL(args, 1);
    return VAL(bool_from_int(lake_equal(a, b)));
}

static LakeVal *_not(LakeList *args)
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

static LakeVal *_add(LakeList *args)
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

static LakeVal *_sub(LakeList *args)
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

static LakeVal *_mul(LakeList *args)
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

static LakeVal *_div(LakeList *args)
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

static LakeVal *_int_eq(LakeList *args)
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

static LakeVal *_int_lt(LakeList *args)
{
    gboolean result = TRUE;
    size_t n = LIST_N(args);
    size_t i;
    int curr, prev;

    if (n > 1) {
        for (i = 0; i < n; ++i) {
            LakeVal *v = LIST_VAL(args, i);
            ENSURE_INT(v, i);
            curr = INT_VAL(INT(v));
            if (i > 0) {
                result = result && prev < curr;
            }
            prev = INT_VAL(INT(v));
        }
    }
    return VAL(bool_from_int(result));
}

static LakeVal *_int_gt(LakeList *args)
{
    gboolean result = TRUE;
    size_t n = LIST_N(args);
    size_t i;
    int curr, prev;

    if (n > 1) {
        for (i = 0; i < n; ++i) {
            LakeVal *v = LIST_VAL(args, i);
            ENSURE_INT(v, i);
            curr = INT_VAL(INT(v));
            if (i > 0) {
                result = result && prev > curr;
            }
            prev = INT_VAL(INT(v));
        }
    }
    return VAL(bool_from_int(result));
}

Env *primitive_bindings(void)
{
    #define DEFINE(name, fn, arity) env_define(env, sym_intern(name), VAL(prim_make(name, arity, fn)))
    
    Env *env = env_toplevel();
    DEFINE("car", _car, 1);
    DEFINE("cdr", _cdr, 1);
    DEFINE("cons", _cons, 2);
    DEFINE("null?", _nullP, 1);
    DEFINE("pair?", _pairP, 1);
    DEFINE("is?", _isP, 2);
    DEFINE("equal?", _equalP, 2);
    DEFINE("not", _not, 1);
    DEFINE("+", _add, ARITY_VARARGS);
    DEFINE("-", _sub, ARITY_VARARGS);
    DEFINE("*", _mul, ARITY_VARARGS);
    DEFINE("/", _div, ARITY_VARARGS);
    DEFINE("=", _int_eq, ARITY_VARARGS);
    DEFINE("<", _int_lt, ARITY_VARARGS);
    DEFINE(">", _int_gt, ARITY_VARARGS);

    /* symbol? */
    /* list? */
    /* dotted-list? */
    /* number? */
    /* integer? */
    /* string? */
    /* bool? */
    /* function? */
    /* primitive? */

    /* string=? */
    /* string< */
    /* string> */
    /* string-concatenate */
    /* string-slice */
    
    return env;
}