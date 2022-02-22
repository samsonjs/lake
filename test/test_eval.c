/**
 * test_eval.c
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

void setup(void);
static char *test_eval(void);
static char *test_eval_exprs(void);
static char *test_eval_exprs1(void);
static char *test_apply(void);

int tests_run;
char *failed_test;

static LakeCtx *lake;
static LakeSym *s_car;
static LakeSym *s_cdr;
static LakePrimitive *p_car;
static LakePrimitive *p_cdr;

int main(int argc, char const *argv[])
{
    setup();
    return !lt_run_tests("Eval & Apply",
                         (test_fn[]){test_eval, test_eval_exprs,
                                     test_eval_exprs1, test_apply, NULL});
}

void setup(void)
{
    lake = lake_init();
    s_car = sym_intern(lake, "car");
    s_cdr = sym_intern(lake, "cdr");
    p_car = PRIM(eval(lake, lake->toplevel, VAL(s_car)));
    p_cdr = PRIM(eval(lake, lake->toplevel, VAL(s_cdr)));
}

/* LakeList *eval_exprs(LakeCtx *ctx, Env *env, LakeList *exprs) */
static char *test_eval_exprs(void)
{
    LakeList *exprs = list_make();
    LakeList *results;

    /* no expressions */
    results = eval_exprs(lake, lake->toplevel, exprs);
    lt_assert("non-empty result list when evaluating no expressions",
              0 == LIST_N(results));
    list_free(results);

    /* one expression */
    list_append(exprs, VAL(s_car));
    results = eval_exprs(lake, lake->toplevel, exprs);
    lt_assert("expected one result", 1 == LIST_N(results));
    lt_assert("expected the primitive car", VAL(p_car) == LIST_VAL(results, 0));
    list_free(results);

    /* two expressions */
    list_append(exprs, VAL(s_cdr));
    results = eval_exprs(lake, lake->toplevel, exprs);
    lt_assert("expected two results", 2 == LIST_N(results));
    lt_assert("expected the primitive car", VAL(p_car) == LIST_VAL(results, 0));
    lt_assert("expected the primitive cdr", VAL(p_cdr) == LIST_VAL(results, 1));
    list_free(results);

    list_free(exprs);

    return 0;
}

/* LakeVal *eval_exprs1(LakeCtx *ctx, Env *env, LakeList *exprs) */
static char *test_eval_exprs1(void)
{
    LakeList *exprs = list_make();
    LakeVal *result;

    /* no expressions */
    result = eval_exprs1(lake, lake->toplevel, exprs);
    lt_assert("expected NULL when evaluating no expressions", NULL == result);

    /* one expression */
    list_append(exprs, VAL(s_car));
    result = eval_exprs1(lake, lake->toplevel, exprs);
    lt_assert("expected the primitive car", VAL(p_car) == result);

    /* two expressions */
    list_append(exprs, VAL(s_cdr));
    result = eval_exprs1(lake, lake->toplevel, exprs);
    lt_assert("expected the primitive cdr", VAL(p_cdr) == result);

    list_free(exprs);

    return 0;
}

#define EVAL(x) (eval(lake, lake->toplevel, VAL(x)))

/* LakeVal *eval(LakeCtx *ctx, Env *env, LakeVal *expr) */
static char *test_eval(void)
{
    LakeComment *l_comment = comment_from_c("");
    lt_assert("comment does not evaluate to nothing", NULL == EVAL(l_comment));

    LakeBool *l_bool = lake->T;
    LakeInt *l_int = int_from_c(42);
    LakeStr *l_str = lake_str_from_c("i am the walrus");
    lt_assert("bool does not self evaluate", VAL(l_bool) == EVAL(l_bool));
    lt_assert("int does not self evaluate", VAL(l_int) == EVAL(l_int));
    lt_assert("string does not self evaluate", VAL(l_str) == EVAL(l_str));

    LakeSym *isP = sym_intern(lake, "is?");
    LakeSym *l_bound_sym = isP;
    LakeSym *l_unbound_sym = sym_intern(lake, "sex");
    lt_assert("bound symbol is? evaluated to null", NULL != EVAL(l_bound_sym));
    lt_assert("unbound symbol evaluated to non-null",
              NULL == EVAL(l_unbound_sym));

    LakeList *l_call = list_make();
    lt_assert("empty list (nil) did not self evaluate",
              VAL(l_call) == EVAL(l_call));

    LakeDottedList *l_dlist = dlist_make(list_make(), VAL(l_int));
    lt_assert("dotted-list evaluated to non-null", NULL == EVAL(l_dlist));

    list_append(l_call, VAL(sym_intern(lake, "define")));
    LakeSym *s_x = sym_intern(lake, "x");
    list_append(l_call, VAL(s_x));
    list_append(l_call, VAL(l_int));
    lt_assert("define special form evaluated to non-null",
              NULL == EVAL(l_call));
    lt_assert("define bound an incorrect value", VAL(l_int) == EVAL(s_x));
    list_free(l_call);

    l_call = list_make();
    list_append(l_call, VAL(isP));
    list_append(l_call, VAL(s_x));
    list_append(l_call, VAL(l_int));
    lt_assert("primitive evaluated incorrectly",
              lake_is_true(lake, EVAL(l_call)));
    list_free(l_call);

    return 0;
}

/* LakeVal *apply(LakeCtx *ctx, LakeVal *fnVal, LakeList *args) */
static char *test_apply(void)
{
    LakeVal *fnVal = VAL(p_car);
    LakeList *args = list_make();

    /* apply primitive with too few args */
    lt_assert("called primitive with too few args",
              NULL == apply(lake, fnVal, args));

    /* apply primitive with correct # of args */
    LakeList *list = list_make();
    LakeSym *sym = sym_intern(lake, "test");
    list_append(list, VAL(sym));
    list_append(args, VAL(list));
    lt_assert("car of '(test) is not the symbol test",
              VAL(sym) == apply(lake, fnVal, args));

    /* apply primitive with too many args */
    list_append(args, VAL(sym));
    lt_assert("called primitive with too many args",
              NULL == apply(lake, fnVal, args));
    list_free(args);

    /* var args primitive */
    fnVal = EVAL(sym_intern(lake, "+"));
    args = list_make();
    lt_assert("failed to call var args primitive (+)",
              0 == INT_VAL(INT(apply(lake, fnVal, args))));
    list_append(args, VAL(int_from_c(1)));
    lt_assert("failed to call var args primitive (+ 1)",
              1 == INT_VAL(INT(apply(lake, fnVal, args))));
    list_append(args, VAL(int_from_c(2)));
    lt_assert("failed to call var args primitive (+ 1 2)",
              3 == INT_VAL(INT(apply(lake, fnVal, args))));
    list_append(args, VAL(int_from_c(3)));
    lt_assert("failed to call var args primitive (+ 1 2 3)",
              6 == INT_VAL(INT(apply(lake, fnVal, args))));
    list_free(args);

    /* set up a scheme function with fixed args */
    eval(lake, lake->toplevel,
         parse_expr(lake, "(define zero? (lambda (x) (= x 0)))", 35));
    fnVal = EVAL(sym_intern(lake, "zero?"));
    args = list_make();

    /* apply lambda with too few args */
    lt_assert("function applied incorrectly", NULL == apply(lake, fnVal, args));

    /* apply lambda with correct # of args */
    list_append(args, VAL(int_from_c(0)));
    lt_assert("function applied incorrectly",
              VAL(lake->T) == apply(lake, fnVal, args));

    /* apply lambda with too many args */
    list_append(args, VAL(int_from_c(0)));
    lt_assert("function applied incorrectly", NULL == apply(lake, fnVal, args));
    list_free(args);

    /* set up a scheme function with only var args */
    eval(lake, lake->toplevel,
         parse_expr(lake, "(define list (lambda rest rest))", 32));
    fnVal = EVAL(sym_intern(lake, "list"));
    args = list_make();

    /* apply lambda with too few args */
    lt_assert("var args function applied incorrectly",
              NULL != apply(lake, fnVal, args));

    /* apply lambda with correct # of args */
    list_append(args, VAL(int_from_c(0)));
    lt_assert("var args function applied incorrectly",
              NULL != apply(lake, fnVal, args));

    /* apply lambda with too many args */
    list_append(args, VAL(int_from_c(1)));
    lt_assert("var args function applied incorrectly",
              NULL != apply(lake, fnVal, args));
    list_free(args);

    /* set up a scheme function with fixed and var args */
    eval(lake, lake->toplevel,
         parse_expr(lake, "(define frob (lambda (a b . rest) b))", 37));
    fnVal = EVAL(sym_intern(lake, "frob"));
    args = list_make();

    /* apply var args lambda with too few args */
    lt_assert("var args function applied incorrectly",
              NULL == apply(lake, fnVal, args));
    list_append(args, VAL(int_from_c(0)));
    lt_assert("var args function applied incorrectly",
              NULL == apply(lake, fnVal, args));

    /* apply var args lambda with minimum # of args */
    list_append(args, VAL(int_from_c(1)));
    lt_assert("var args function applied incorrectly",
              NULL != apply(lake, fnVal, args));

    /* apply lambda with extra args */
    list_append(args, VAL(int_from_c(2)));
    lt_assert("var args function applied incorrectly",
              NULL != apply(lake, fnVal, args));
    list_append(args, VAL(int_from_c(3)));
    lt_assert("var args function applied incorrectly",
              NULL != apply(lake, fnVal, args));
    list_free(args);

    /* non-function in head position */
    lt_assert("apply with non-function returned non-null",
              NULL == apply(lake, VAL(sym), list_make()));

    return 0;
}
