/** 
  * lake.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  * A quick and dirty scheme written in C, for fun and to use while
  * reading The Little Schemer.
  *
  */

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include "env.h"
#include "eval.h"
#include "lake.h"
#include "list.h"
#include "parse.h"
#include "primitive.h"
#include "string.h"

static LakeBool _T = { { TYPE_BOOL, sizeof(LakeBool) }, TRUE };
static LakeBool _F = { { TYPE_BOOL, sizeof(LakeBool) }, FALSE };
LakeBool *T = &_T;
LakeBool *F = &_F;

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

static LakeVal *prim_add(LakeList *args)
{
    int result = 0;
    size_t n = LIST_N(args);
    size_t i;
    for (i = 0; i < n; ++i) {
        LakeVal *v = LIST_VAL(args, i);
        if (!IS(TYPE_INT, v)) {
            ERR("argument %zu is not an integer: %s", i, repr(v));
            return NULL;
        }
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
        if (!IS(TYPE_INT, v)) {
            ERR("argument %zu is not an integer: %s", i, repr(v));
            return NULL;
        }
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
        if (!IS(TYPE_INT, v)) {
            ERR("argument %zu is not an integer: %s", i, repr(v));
            return NULL;
        }
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
    if (!IS(TYPE_INT, v)) {
        ERR("argument 0 is not an integer: %s", repr(v));
        return NULL;
    }
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
            if (!IS(TYPE_INT, v)) {
                ERR("argument %zu is not an integer: %s", i, repr(v));
                return NULL;
            }
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

static Env *primitive_bindings(void)
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
    return env;
}

void print(LakeVal *val)
{
    puts(repr(val));
}

static LakeVal *prompt_read(char *prompt)
{
    static int n = 1024;
    printf("%s", prompt);
    char buf[n];
    if (!fgets(buf, n, stdin)) {
        if (ferror(stdin)) {
            printf("error: cannot read from stdin");
        }
        if (feof(stdin)) {
            return VAL(EOF);
        }
        return NULL;
    }
	/* trim the newline if any */
	buf[strcspn(buf, "\n")] = '\0';
    return parse_expr(buf, strlen(buf));
}

static void run_repl_with_env(Env *env)
{
    puts("Lake Scheme v" LAKE_VERSION);
    LakeVal *expr;
    LakeVal *result;
    for (;;) {
        expr = prompt_read("> ");
        if (expr == VAL(EOF)) break;
        if (expr == VAL(PARSE_ERR)) {
            ERR("parse error");
            continue;
        }
        if (expr) {
            result = eval(env, expr);
            if (result) print(result);
        }
    }
}

static void run_repl(void)
{
    run_repl_with_env(primitive_bindings());
}

static void run_one_then_repl(int argc, char const *args[])
{
    /* create a top level environment */
    Env *env = primitive_bindings();

    /* bind args */
    /*
    LakeList *lakeArgs = list_of_strings_from_array(argc, args);
    env_bind(env, "args", lakeArgs);
    */

    /* eval (load args[0]) in env */
    
    run_repl_with_env(env);
}

char *repr(LakeVal *expr)
{
    if (expr == NULL) return g_strdup("(null)");

    char *s = NULL;
    LakeStr *str;
    
    switch (expr->type) {

    case TYPE_SYM:
        s = sym_repr(SYM(expr));
        break;

    case TYPE_BOOL:
        s = bool_repr(BOOL(expr));
        break;

    case TYPE_INT:
        str = int_to_str(INT(expr));
        s = str_val(str);
        str_free(str);
        break;

    case TYPE_STR:
        s = g_strdup_printf("\"%s\"", STR_S(STR(expr)));
        break;

    case TYPE_LIST:
		s = list_repr(LIST(expr));
        break;
    
    case TYPE_DLIST:
        s = dlist_repr(DLIST(expr));
        break;
    
    case TYPE_FN:
        s = fn_repr(FN(expr));
        break;
    
    default:
        printf("error: unrecognized value, type %d, size %zu bytes", expr->type, expr->size);
        s = g_strdup("");
    }
    
    return s;
}

int main (int argc, char const *argv[])
{
    if (argc == 1) {
        run_repl();
    } else {
        run_one_then_repl(argc, argv);
    }
    return 0;
}
