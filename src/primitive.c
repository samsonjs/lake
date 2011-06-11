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
#include "bool.h"
#include "common.h"
#include "comment.h"
#include "dlist.h"
#include "env.h"
#include "int.h"
#include "dlist.h"
#include "fn.h"
#include "list.h"
#include "lake.h"
#include "primitive.h"
#include "str.h"

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

static LakeVal *_car(LakeCtx *ctx, LakeList *args)
{
    LakeList *list = LIST(LIST_VAL(args, 0));
    if (lk_is_type(TYPE_LIST, list) && LIST_N(list) > 0) {
        return LIST_VAL(list, 0);
    }
    if (lk_is_type(TYPE_DLIST, list)) {
        return VAL(dlist_head(DLIST(list)));
    }
    ERR("not a pair: %s", lake_repr(list));
    return NULL;
}

static LakeVal *_cdr(LakeCtx *ctx, LakeList *args)
{
    LakeList *list = LIST(LIST_VAL(args, 0));
    if (lk_is_type(TYPE_LIST, list) && LIST_N(list) > 0) {
        LakeList *cdr = list_copy(list);
        list_shift(cdr);
        return VAL(cdr);
    }
    if (lk_is_type(TYPE_DLIST, list)) {
        return dlist_tail(DLIST(list));
    }
    ERR("not a pair: %s", lake_repr(list));
    return NULL;
}

static LakeVal *_cons(LakeCtx *ctx, LakeList *args)
{
    LakeVal *car = LIST_VAL(args, 0);
    LakeVal *cdr = LIST_VAL(args, 1);
    return VAL(list_cons(car, cdr));
}

static LakeVal *_nullP(LakeCtx *ctx, LakeList *args)
{
    LakeVal *val = list_shift(args);
    LakeBool *is_null = lk_bool_from_int(ctx, lk_is_type(TYPE_LIST, val) && LIST_N(LIST(val)) == 0);
    return VAL(is_null);
}

static LakeVal *_pairP(LakeCtx *ctx, LakeList *args)
{
    LakeVal *val = list_shift(args);
    LakeBool *is_pair = lk_bool_from_int(ctx, lk_is_type(TYPE_LIST, val) && LIST_N(LIST(val)) > 0);
    return VAL(is_pair);
}

static LakeVal *_isP(LakeCtx *ctx, LakeList *args)
{
    LakeVal *a = LIST_VAL(args, 0);
    LakeVal *b = LIST_VAL(args, 1);
    return VAL(lk_bool_from_int(ctx, lake_is(a, b)));
}

static LakeVal *_equalP(LakeCtx *ctx, LakeList *args)
{
    LakeVal *a = LIST_VAL(args, 0);
    LakeVal *b = LIST_VAL(args, 1);
    return VAL(lk_bool_from_int(ctx, lake_equal(a, b)));
}

static LakeVal *_not(LakeCtx *ctx, LakeList *args)
{
    LakeVal *val = list_shift(args);
    LakeBool *not = lk_bool_from_int(ctx, lk_is_false(ctx, val));
    return VAL(not);
}

#define ENSURE_INT(x, i) do {                                           \
        if (!lk_is_type(TYPE_INT, x)) {                                         \
            ERR("argument %zu is not an integer: %s", i, lake_repr(x)); \
            return NULL;                                                \
        }                                                               \
    } while (0)

static LakeVal *_add(LakeCtx *ctx, LakeList *args)
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

static LakeVal *_sub(LakeCtx *ctx, LakeList *args)
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

static LakeVal *_mul(LakeCtx *ctx, LakeList *args)
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

static LakeVal *_div(LakeCtx *ctx, LakeList *args)
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

static LakeVal *_int_eq(LakeCtx *ctx, LakeList *args)
{
    bool result = TRUE;
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
    return VAL(lk_bool_from_int(ctx, result));
}

static LakeVal *_int_lt(LakeCtx *ctx, LakeList *args)
{
    bool result = TRUE;
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
    return VAL(lk_bool_from_int(ctx, result));
}

static LakeVal *_int_gt(LakeCtx *ctx, LakeList *args)
{
    bool result = TRUE;
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
    return VAL(lk_bool_from_int(ctx, result));
}

static LakeVal *_set_carB(LakeCtx *ctx, LakeList *args)
{
    LakeList *list = LIST(LIST_VAL(args, 0));
    if (lk_is_type(TYPE_LIST, list)) {
        LakeVal *new_car = LIST_VAL(args, 1);
        if (LIST_N(list) == 0) {
            list_append(list, new_car);
        }
        else {
            list_set(list, 0, new_car);
        }
        return VAL(list);
    }
    ERR("not a pair: %s", lake_repr(list));
    return NULL;
}

static LakeVal *_display(LakeCtx *ctx, LakeList *args)
{
    size_t n = LIST_N(args);
    size_t i;
    int space = 0;
    for (i = 0; i < n; ++i) {
        if (space) putchar(' ');
        printf("%s", lake_repr(LIST_VAL(args, i)));
        space = 1;
    }
    putchar('\n');
    return NULL;
}

#define DEFINE_PREDICATE(name, type)                                            \
    static LakeVal *_## name ##P(LakeCtx *ctx, LakeList *args)                  \
    {                                                                           \
        return VAL(lk_bool_from_int(ctx, lk_is_type(type, LIST_VAL(args, 0)))); \
    }

DEFINE_PREDICATE(symbol, TYPE_SYM)
DEFINE_PREDICATE(list, TYPE_LIST)
DEFINE_PREDICATE(dotted_list, TYPE_DLIST)
DEFINE_PREDICATE(number, TYPE_INT)
DEFINE_PREDICATE(integer, TYPE_INT)
DEFINE_PREDICATE(string, TYPE_STR)
DEFINE_PREDICATE(bool, TYPE_BOOL)
DEFINE_PREDICATE(function, TYPE_FN)
DEFINE_PREDICATE(primitive, TYPE_PRIM)

#undef DEFINE_PREDICATE

void bind_primitives(LakeCtx *ctx)
{
    #define DEFINE(name, fn, arity) env_define(ctx->toplevel,         \
                                               sym_intern(ctx, name), \
                                               VAL(prim_make(name, arity, fn)))

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
    DEFINE("set-car!", _set_carB, 2);

    DEFINE("display", _display, ARITY_VARARGS);

    DEFINE("symbol?", _symbolP, 1);
    DEFINE("list?", _listP, 1);
    DEFINE("dotted-list?", _dotted_listP, 1);
    DEFINE("number?", _numberP, 1);
    DEFINE("integer?", _integerP, 1);
    DEFINE("string?", _stringP, 1);
    DEFINE("bool?", _boolP, 1);
    DEFINE("function?", _functionP, 1);
    DEFINE("primitive?", _primitiveP, 1);

    /* string=? */
    /* string< */
    /* string> */
    /* string-concatenate */
    /* string-slice */

    #undef DEFINE
}
