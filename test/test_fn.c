/**
 * test_fn.c
 * Lake Scheme
 *
 * Copyright 2011 Sami Samhuri
 * MIT License
 *
 */

#include "env.h"
#include "eval.h"
#include "lake.h"
#include "laketest.h"
#include "parse.h"
#include <string.h>

static char *test_fn_make(void);
static char *test_fn_repr(void);

int main(int argc, char const *argv[])
{
    return !lt_run_tests("Functions",
                         (test_fn[]){test_fn_make, test_fn_repr, NULL});
}

/* LakeFn *fn_make(LakeList *params, LakeSym *varargs, LakeList *body, Env
 * *closure) */
static char *test_fn_make(void)
{
    LakeList *params = list_make();
    LakeSym *varargs = NULL;
    LakeList *body = list_make();
    Env *closure = env_make(NULL);
    LakeFn *fn = fn_make(params, varargs, body, closure);
    lt_assert("fn_make returned null", NULL != fn);
    lt_assert("fn_make did not return a function", TYPE_FN == fn->base.type);
    lt_assert("function has incorrect size", sizeof(LakeFn) == fn->base.size);
    lt_assert("params was set incorrectly", params == fn->params);
    lt_assert("varargs was set incorrectly", varargs == fn->varargs);
    lt_assert("body was set incorrectly", body == fn->body);
    lt_assert("closure was set incorrectly", closure == fn->closure);
    return 0;
}

static LakeVal *compile(LakeCtx *lake, char *s)
{
    return eval(lake, lake->toplevel, parse_expr(lake, s, strlen(s)));
}

/* char *fn_repr(LakeFn *fn) */
static char *test_fn_repr(void)
{
    static char *PLUS1_REPR = "(lambda (x) (+ x 1))";
    static char *LIST_REPR = "(lambda rest rest)";
    static char *SILLY_REPR = "(lambda (a b . rest) (+ a b) rest)";

    LakeCtx *lake = lake_init();

    LakeFn *plus1_fn = FN(compile(lake, PLUS1_REPR));
    lt_assert("plus1 function representation is incorrect",
              strncmp(PLUS1_REPR, fn_repr(plus1_fn), strlen(PLUS1_REPR)) == 0);

    LakeFn *list_fn = FN(compile(lake, LIST_REPR));
    lt_assert("list function representation is incorrect",
              strncmp(LIST_REPR, fn_repr(list_fn), strlen(LIST_REPR)) == 0);

    LakeFn *silly_fn = FN(compile(lake, SILLY_REPR));
    lt_assert("silly function representation is incorrect",
              strncmp(SILLY_REPR, fn_repr(silly_fn), strlen(SILLY_REPR)) == 0);

    return 0;
}
