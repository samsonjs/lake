/** 
  * eval.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include "env.h"
#include "eval.h"
#include "fn.h"
#include "lake.h"
#include "symtable.h"

typedef LakeVal *(*special_form_handler)(Env *env, LakeList *expr);
static GHashTable *special_form_handlers = NULL;
static void init_special_form_handlers(void);

static void invalid_special_form(LakeList *expr, char *detail)
{
    ERR("malformed special form, %s: %s", detail, repr(VAL(expr)));
}

/* expr begins with the symbol "quote" so the quoted value is the 2nd value */
static LakeVal *_quote(Env *env, LakeList *expr)
{
    if (LIST_N(expr) == 2) {
        return list_pop(expr);
    }
    invalid_special_form(expr, "quote requires exactly one parameter");
    return NULL;
}

static LakeVal *_and(Env *env, LakeList *expr)
{
    /* drop the "and" symbol */
    list_shift(expr);
    
    /* (and ...) */
    LakeVal *result = LIST_N(expr) ? eval(env, list_shift(expr)) : VAL(T);
    while (IS_TRUTHY(result) && LIST_N(expr) > 0) {
        result = bool_and(result, eval(env, list_shift(expr)));
    }
    return result;
}

static LakeVal *_or(Env *env, LakeList *expr)
{
    /* drop the "or" symbol */
    list_shift(expr);
    
    /* (or ...) */
    LakeVal *result = LIST_N(expr) ? eval(env, list_shift(expr)) : VAL(F);
    while (IS_FALSY(result) && LIST_N(expr) > 0) {
        result = bool_or(result, eval(env, list_shift(expr)));
    }
    return result;
}

static LakeVal *_setB(Env *env, LakeList *expr)
{
    /* (set! x 42) */
    if (LIST_N(expr) == 3 && IS(TYPE_SYM, LIST_VAL(expr, 1))) {
        list_shift(expr); /* drop the "set!" symbol */
        LakeSym *var = SYM(list_shift(expr));
        LakeVal *form = list_shift(expr);
        if (!env_set(env, var, form)) {
            ERR("%s is not defined", SYM_S(var));
        }
    }
    else {
        invalid_special_form(expr, "set! requires exactly 2 parameters");
    }
    return NULL;
}

static LakeVal *_define(Env *env, LakeList *expr)
{
    /* TODO: make these more robust, check all expected params */
    
    /* (define x 42) */
    if (LIST_N(expr) == 3 && IS(TYPE_SYM, LIST_VAL(expr, 1))) {
        list_shift(expr); /* drop the "define" symbol */
        LakeSym *var = SYM(list_shift(expr));
        LakeVal *form = list_shift(expr);
        env_define(env, var, eval(env, form));
    }
    
    /* (define (inc x) (+ 1 x)) */
    else if (LIST_N(expr) >= 3 && IS(TYPE_LIST, LIST_VAL(expr, 1))) {
        list_shift(expr); /* drop the "define" symbol */
        LakeList *params = LIST(list_shift(expr));
        LakeSym *var = SYM(list_shift(params));
        LakeList *body = expr;
        env_define(env, var, VAL(fn_make(params, NULL, body, env)));
    }
    
    /* (define (print format . args) (...)) */
    else if (LIST_N(expr) >= 3 && IS(TYPE_DLIST, LIST_VAL(expr, 1))) {
        list_shift(expr); /* drop the "define" symbol */
        LakeDottedList *def = DLIST(list_shift(expr));
        LakeList *params = DLIST_HEAD(def);
        LakeSym *varargs = SYM(DLIST_TAIL(def));
        LakeSym *var = SYM(list_shift(params));
        LakeList *body = expr;
        env_define(env, var, VAL(fn_make(params, varargs, body, env)));
    }
    
    else {
        invalid_special_form(expr, "define requires at least 2 parameters");
    }
    
    return NULL;
}

static LakeVal *_lambda(Env *env, LakeList *expr)
{
    /* (lambda (a b c) ...) */
    if (LIST_N(expr) >= 3 && IS(TYPE_LIST, LIST_VAL(expr, 1))) {
        list_shift(expr); /* drop the "lambda" symbol */
        LakeList *params = LIST(list_shift(expr));
        LakeList *body = expr;
        return VAL(fn_make(params, NULL, body, env));
    }
    else if (LIST_N(expr) >= 3 && IS(TYPE_DLIST, LIST_VAL(expr, 1))) {
        list_shift(expr); /* drop the "lambda" symbol */
        LakeDottedList *def = DLIST(list_shift(expr));
        LakeList *params = DLIST_HEAD(def);
        LakeSym *varargs = SYM(DLIST_TAIL(def));
        LakeList *body = expr;
        return VAL(fn_make(params, varargs, body, env));
    }
    else if (LIST_N(expr) >= 3 && IS(TYPE_SYM, LIST_VAL(expr, 1))) {
        list_shift(expr); /* drop the "lambda" symbol */
        LakeSym *varargs = SYM(list_shift(expr));
        LakeList *body = expr;
        return VAL(fn_make(list_make(), varargs, body, env));
    }
    else {
        invalid_special_form(expr, "lambda requires at least 2 parameters");
        return NULL;
    }
}

static LakeVal *_if(Env *env, LakeList *expr)
{
    if (LIST_N(expr) < 3) {
        invalid_special_form(expr, "if requires 3 parameters");
        return NULL;
    }
    list_shift(expr); /* "if" token */
    LakeVal *cond = eval(env, list_shift(expr));
    if (IS_TRUTHY(cond)) {
        return eval(env, list_shift(expr));
    }
    else {
        return eval(env, LIST_VAL(expr, 1));
    }
}

static void init_special_form_handlers(void)
{
    #define HANDLER(name, fn) g_hash_table_insert(special_form_handlers, \
                                                  sym_intern(name),      \
                                                  (gpointer)fn)

    special_form_handlers = symtable_make();
    /* HANDLER("load", &load_special_form); */
    /* HANDLER("cond", &cond_special_form); */
    HANDLER("quote", &_quote);
    HANDLER("and", &_and);
    HANDLER("or", &_or);
    HANDLER("if", &_if);
    /* HANDLER("when", &_when); */
    HANDLER("set!", &_setB);
    HANDLER("define", &_define);
    HANDLER("lambda", &_lambda);
    /* HANDLER("let", &_let); */
    /* HANDLER("let!", &_letB); */
    /* HANDLER("letrec", &_letrec); */
}

gboolean is_special_form(LakeList *expr)
{
    if (special_form_handlers == NULL) {
        init_special_form_handlers();
    }
    
    LakeVal *head = LIST_VAL(expr, 0);
    if (!IS(TYPE_SYM, head)) return FALSE;
    GList *special_form_names = g_hash_table_get_keys(special_form_handlers);
    return !!g_list_find(special_form_names, SYM(head));
}

static special_form_handler get_special_form_handler(LakeSym *name)
{
    if (special_form_handlers == NULL) {
        init_special_form_handlers();
    }
    return (special_form_handler)g_hash_table_lookup(special_form_handlers, name);
}

static LakeVal *eval_special_form(Env *env, LakeList *expr)
{
    LakeSym *name = SYM(LIST_VAL(expr, 0));
    special_form_handler handler = get_special_form_handler(name);
    if (handler) {
        return handler(env, list_copy(expr));
    }
    ERR("unrecognized special form: %s", SYM_S(name));
    return NULL;
}

LakeVal *eval(Env *env, LakeVal *expr)
{
    LakeVal *result;
    LakeList *list;
    
    switch (expr->type) {

    /* self evaluating types */
    case TYPE_BOOL:
    case TYPE_INT:
    case TYPE_STR:
        result = expr;
        break;

    case TYPE_SYM:
        result = env_get(env, (gpointer)SYM(expr));
        break;

    case TYPE_DLIST:
        ERR("malformed function call");
        result = NULL;
        break;

    case TYPE_COMM:
        result = NULL;
        break;

    case TYPE_LIST:
        list = LIST(expr);

        if (LIST_N(list) == 0) {
            result = expr;
        }
        else {
            if (is_special_form(list)) {
                result = eval_special_form(env, list);
            }
            else {
                LakeVal *fn = eval(env, LIST_VAL(list, 0));
                if (!fn) {
                    return NULL;
                }
                LakeList *args = list_make_with_capacity(LIST_N(list) - 1);
                int i;
                LakeVal *v;
                for (i = 1; i < LIST_N(list); ++i) {
                    v = eval(env, LIST_VAL(list, i));
                    if (v != NULL) {
                        list_append(args, v);
                    }
                    else {
                        list_free(args);
                        result = NULL;
                        goto done;
                    }
                }
                result = apply(fn, args);
            }
        }
        break;

    default:
        ERR("unrecognized value, type %d, size %zu bytes", expr->type, expr->size);
        DIE("we don't eval that around here!");
    }
    
    done: return result;
}

LakeList *eval_exprs(Env *env, LakeList *exprs)
{
    LakeList *results = list_make_with_capacity(LIST_N(exprs));
    int i;
    for (i = 0; i < LIST_N(exprs); ++i) {
        list_append(results, eval(env, LIST_VAL(exprs, i)));
    }
    return results;
}

LakeVal *apply(LakeVal *fnVal, LakeList *args)
{
    LakeVal *result = NULL;
    if (IS(TYPE_PRIM, fnVal)) {
        LakePrimitive *prim = PRIM(fnVal);
        int arity = prim->arity;
        if (arity == ARITY_VARARGS || LIST_N(args) == arity) {
            result = prim->fn(args);
        }
        else {
            ERR("%s expects %d params but got %zu", prim->name, arity, LIST_N(args));
            result = NULL;
        }
    }
    else if (IS(TYPE_FN, fnVal)) {
        LakeFn *fn = FN(fnVal);
        
        /* Check # of params */
        size_t nparams = LIST_N(fn->params);
        if (nparams != LIST_N(args) && !fn->varargs) {
            ERR("expected %zu params but got %zu", nparams, LIST_N(args));
            return NULL;
        }

        Env *env = env_make(fn->closure);

        /* bind each (param,arg) pair in env */
        size_t i;
        for (i = 0; i < nparams; ++i) {
            env_define(env, SYM(LIST_VAL(fn->params, i)), LIST_VAL(args, i));
        }

        /* bind varargs */
        if (fn->varargs) {
            if (LIST_N(args) < nparams) {
                ERR("expected at least %zu params but got %zu", nparams, LIST_N(args));
                return NULL;
            }
            LakeList *remainingArgs = list_make_with_capacity(LIST_N(args) - nparams);
            for (; i < LIST_N(args); ++i) {
                list_append(remainingArgs, LIST_VAL(args, i));
            }
            env_define(env, fn->varargs, VAL(remainingArgs));
        }

        /* evaluate body */
        LakeList *results = eval_exprs(env, fn->body);
        result = list_pop(results);
        list_free(results);
    }
    else {
        ERR("not a function: %s", repr(fnVal));
    }
    return result;
}
