/**
 * eval.c
 * Lake Scheme
 *
 * Copyright 2011 Sami Samhuri
 * MIT License
 *
 */

#include "eval.h"
#include "bool.h"
#include "common.h"
#include "env.h"
#include "fn.h"
#include "lake.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef LakeVal *(*special_form_handler)(LakeCtx *ctx, Env *env,
                                         LakeList *expr);

static void invalid_special_form(LakeList *expr, char *detail)
{
    ERR("malformed special form, %s: %s", detail, lake_repr(expr));
}

/* expr begins with the symbol "quote" so the quoted value is the 2nd value */
static LakeVal *_quote(LakeCtx *ctx, Env *env, LakeList *expr)
{
    if (LIST_N(expr) == 2)
    {
        return list_pop(expr);
    }
    invalid_special_form(expr, "quote requires exactly one parameter");
    return NULL;
}

static LakeVal *_and(LakeCtx *ctx, Env *env, LakeList *expr)
{
    /* drop the "and" symbol */
    list_shift(expr);

    /* (and ...) */
    LakeVal *result =
        LIST_N(expr) ? eval(ctx, env, list_shift(expr)) : VAL(ctx->T);
    while (lake_is_truthy(ctx, result) && LIST_N(expr) > 0)
    {
        result = lake_bool_and(ctx, result, eval(ctx, env, list_shift(expr)));
    }
    return result;
}

static LakeVal *_or(LakeCtx *ctx, Env *env, LakeList *expr)
{
    /* drop the "or" symbol */
    list_shift(expr);

    /* (or ...) */
    LakeVal *result =
        LIST_N(expr) ? eval(ctx, env, list_shift(expr)) : VAL(ctx->F);
    while (lake_is_falsy(ctx, result) && LIST_N(expr) > 0)
    {
        result = lake_bool_or(ctx, result, eval(ctx, env, list_shift(expr)));
    }
    return result;
}

static LakeVal *_setB(LakeCtx *ctx, Env *env, LakeList *expr)
{
    /* (set! x 42) */
    if (LIST_N(expr) == 3 && lake_is_type(TYPE_SYM, LIST_VAL(expr, 1)))
    {
        list_shift(expr); /* drop the "set!" symbol */
        LakeSym *var = SYM(list_shift(expr));
        LakeVal *form = list_shift(expr);
        if (!env_set(env, var, form))
        {
            ERR("%s is not defined", sym_repr(var));
        }
    }
    else
    {
        invalid_special_form(expr, "set! requires exactly 2 parameters");
    }
    return NULL;
}

static LakeVal *_define(LakeCtx *ctx, Env *env, LakeList *expr)
{
    /* TODO: make these more robust, check all expected params */

    /* (define x 42) */
    if (LIST_N(expr) == 3 && lake_is_type(TYPE_SYM, LIST_VAL(expr, 1)))
    {
        list_shift(expr); /* drop the "define" symbol */
        LakeSym *var = SYM(list_shift(expr));
        LakeVal *form = list_shift(expr);
        env_define(env, var, eval(ctx, env, form));
    }

    /* (define (inc x) (+ 1 x)) */
    else if (LIST_N(expr) >= 3 && lake_is_type(TYPE_LIST, LIST_VAL(expr, 1)))
    {
        list_shift(expr); /* drop the "define" symbol */
        LakeList *params = LIST(list_shift(expr));
        LakeSym *var = SYM(list_shift(params));
        LakeList *body = expr;
        env_define(env, var, VAL(fn_make(params, NULL, body, env)));
    }

    /* (define (print format . args) (...)) */
    else if (LIST_N(expr) >= 3 && lake_is_type(TYPE_DLIST, LIST_VAL(expr, 1)))
    {
        list_shift(expr); /* drop the "define" symbol */
        LakeDottedList *def = DLIST(list_shift(expr));
        LakeList *params = dlist_head(def);
        LakeSym *varargs = SYM(dlist_tail(def));
        LakeSym *var = SYM(list_shift(params));
        LakeList *body = expr;
        env_define(env, var, VAL(fn_make(params, varargs, body, env)));
    }

    else
    {
        invalid_special_form(expr, "define requires at least 2 parameters");
    }

    return NULL;
}

static LakeVal *_lambda(LakeCtx *ctx, Env *env, LakeList *expr)
{
    /* (lambda (a b c) ...) */
    if (LIST_N(expr) >= 3 && lake_is_type(TYPE_LIST, LIST_VAL(expr, 1)))
    {
        list_shift(expr); /* drop the "lambda" symbol */
        LakeList *params = LIST(list_shift(expr));
        LakeList *body = expr;
        return VAL(fn_make(params, NULL, body, env));
    }
    else if (LIST_N(expr) >= 3 && lake_is_type(TYPE_DLIST, LIST_VAL(expr, 1)))
    {
        list_shift(expr); /* drop the "lambda" symbol */
        LakeDottedList *def = DLIST(list_shift(expr));
        LakeList *params = dlist_head(def);
        LakeSym *varargs = SYM(dlist_tail(def));
        LakeList *body = expr;
        return VAL(fn_make(params, varargs, body, env));
    }
    else if (LIST_N(expr) >= 3 && lake_is_type(TYPE_SYM, LIST_VAL(expr, 1)))
    {
        list_shift(expr); /* drop the "lambda" symbol */
        LakeSym *varargs = SYM(list_shift(expr));
        LakeList *body = expr;
        return VAL(fn_make(list_make(), varargs, body, env));
    }
    else
    {
        invalid_special_form(expr, "lambda requires at least 2 parameters");
        return NULL;
    }
}

static LakeVal *_if(LakeCtx *ctx, Env *env, LakeList *expr)
{
    if (LIST_N(expr) != 3)
    {
        invalid_special_form(expr, "if requires 3 parameters");
        return NULL;
    }
    list_shift(expr); /* "if" token */
    LakeVal *cond = eval(ctx, env, list_shift(expr));
    if (lake_is_truthy(ctx, cond))
    {
        return eval(ctx, env, list_shift(expr));
    }
    else
    {
        return eval(ctx, env, LIST_VAL(expr, 1));
    }
}

static LakeVal *_cond(LakeCtx *ctx, Env *env, LakeList *expr)
{
    static LakeVal *ELSE = NULL;
    if (!ELSE) ELSE = VAL(sym_intern(ctx, "else"));

    list_shift(expr); /* "cond" token */
    LakeVal *pred;
    LakeList *conseq;
    while (LIST_N(expr))
    {
        if (!lake_is_type(TYPE_LIST, LIST_VAL(expr, 0)))
        {
            invalid_special_form(expr,
                                 "expected a (predicate consequence) pair");
            return NULL;
        }
        conseq = LIST(list_shift(expr));
        pred = list_shift(conseq);
        if (pred == ELSE || lake_is_truthy(ctx, eval(ctx, env, pred)))
        {
            return eval_exprs1(ctx, env, conseq);
        }
    }
    return NULL;
}

static LakeVal *_when(LakeCtx *ctx, Env *env, LakeList *expr)
{
    if (LIST_N(expr) < 2)
    {
        invalid_special_form(expr, "when requires at least 2 parameters");
        return NULL;
    }
    list_shift(expr); /* "when" token */
    LakeVal *cond = eval(ctx, env, list_shift(expr));
    return lake_is_truthy(ctx, cond) ? eval_exprs1(ctx, env, expr) : NULL;
}

typedef LakeVal *(*handler)(LakeCtx *, Env *, LakeList *);

static void define_handler(LakeCtx *ctx, char *name, handler fn)
{
    lake_hash_put(ctx->special_form_handlers, name, (void *)fn);
}

void init_special_form_handlers(LakeCtx *ctx)
{
    /* define_handler(ctx, "load", &load_special_form); */
    define_handler(ctx, "quote", &_quote);
    define_handler(ctx, "and", &_and);
    define_handler(ctx, "or", &_or);
    define_handler(ctx, "if", &_if);
    define_handler(ctx, "when", &_when);
    define_handler(ctx, "cond", &_cond);
    define_handler(ctx, "set!", &_setB);
    define_handler(ctx, "define", &_define);
    define_handler(ctx, "lambda", &_lambda);
    /* define_handler(ctx, "let", &_let); */
    /* define_handler(ctx, "let!", &_letB); */
    /* define_handler(ctx, "letrec", &_letrec); */
}

bool is_special_form(LakeCtx *ctx, LakeList *expr)
{
    LakeVal *head = LIST_VAL(expr, 0);
    if (!lake_is_type(TYPE_SYM, head)) return FALSE;
    return lake_hash_has(ctx->special_form_handlers, SYM(head)->s);
}

static special_form_handler get_special_form_handler(LakeCtx *ctx,
                                                     LakeSym *name)
{
    return (special_form_handler)lake_hash_get(ctx->special_form_handlers,
                                               name->s);
}

static LakeVal *eval_special_form(LakeCtx *ctx, Env *env, LakeList *expr)
{
    LakeSym *name = SYM(LIST_VAL(expr, 0));
    special_form_handler handler = get_special_form_handler(ctx, name);
    if (handler)
    {
        return handler(ctx, env, list_copy(expr));
    }
    ERR("unrecognized special form: %s", sym_repr(name));
    return NULL;
}

LakeVal *eval_str(LakeCtx *ctx, Env *env, char *s)
{
    return eval(ctx, env, parse_expr(ctx, s, strlen(s)));
}

LakeVal *eval(LakeCtx *ctx, Env *env, LakeVal *expr)
{
    LakeVal *result;
    LakeList *list;

    switch (expr->type)
    {

    /* self evaluating types */
    case TYPE_BOOL:
    case TYPE_INT:
    case TYPE_STR:
        result = expr;
        break;

    case TYPE_SYM:
        result = env_get(env, (void *)SYM(expr));
        if (!result)
        {
            ERR("undefined variable: %s", sym_repr(SYM(expr)));
        }
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

        if (LIST_N(list) == 0)
        {
            result = expr;
        }
        else
        {
            if (is_special_form(ctx, list))
            {
                result = eval_special_form(ctx, env, list);
            }
            else
            {
                LakeVal *fn = eval(ctx, env, LIST_VAL(list, 0));
                if (!fn)
                {
                    return NULL;
                }
                LakeList *args = list_make_with_capacity(LIST_N(list) - 1);
                int i;
                LakeVal *v;
                for (i = 1; i < LIST_N(list); ++i)
                {
                    v = eval(ctx, env, LIST_VAL(list, i));
                    if (v != NULL)
                    {
                        list_append(args, v);
                    }
                    else
                    {
                        list_free(args);
                        result = NULL;
                        goto done;
                    }
                }
                result = apply(ctx, fn, args);
            }
        }
        break;

    default:
        ERR("unrecognized value, type %d, size %zu bytes", expr->type,
            expr->size);
        DIE("we don't eval that around here!");
    }

done:
    return result;
}

LakeList *eval_exprs(LakeCtx *ctx, Env *env, LakeList *exprs)
{
    LakeList *results = list_make_with_capacity(LIST_N(exprs));
    int i;
    for (i = 0; i < LIST_N(exprs); ++i)
    {
        list_append(results, eval(ctx, env, LIST_VAL(exprs, i)));
    }
    return results;
}

LakeVal *eval_exprs1(LakeCtx *ctx, Env *env, LakeList *exprs)
{
    LakeList *results = eval_exprs(ctx, env, exprs);
    LakeVal *result = list_pop(results);
    list_free(results);
    return result;
}

LakeVal *apply(LakeCtx *ctx, LakeVal *fnVal, LakeList *args)
{
    LakeVal *result = NULL;
    if (lake_is_type(TYPE_PRIM, fnVal))
    {
        LakePrimitive *prim = PRIM(fnVal);
        int arity = prim->arity;
        if (arity == ARITY_VARARGS || LIST_N(args) == arity)
        {
            result = prim->fn(ctx, args);
        }
        else
        {
            ERR("%s expects %d params but got %zu", prim->name, arity,
                LIST_N(args));
            result = NULL;
        }
    }
    else if (lake_is_type(TYPE_FN, fnVal))
    {
        LakeFn *fn = FN(fnVal);

        /* Check # of params */
        size_t nparams = LIST_N(fn->params);
        if (!fn->varargs && LIST_N(args) != nparams)
        {
            ERR("expected %zu params but got %zu", nparams, LIST_N(args));
            return NULL;
        }
        else if (fn->varargs && LIST_N(args) < nparams)
        {
            ERR("expected at least %zu params but got %zu", nparams,
                LIST_N(args));
            return NULL;
        }

        Env *env = env_make(fn->closure);

        /* bind each (param,arg) pair in env */
        size_t i;
        for (i = 0; i < nparams; ++i)
        {
            env_define(env, SYM(LIST_VAL(fn->params, i)), LIST_VAL(args, i));
        }

        /* bind varargs */
        if (fn->varargs)
        {
            LakeList *remainingArgs =
                list_make_with_capacity(LIST_N(args) - nparams);
            for (; i < LIST_N(args); ++i)
            {
                list_append(remainingArgs, LIST_VAL(args, i));
            }
            env_define(env, fn->varargs, VAL(remainingArgs));
        }

        /* evaluate body */
        result = eval_exprs1(ctx, env, fn->body);
    }
    else
    {
        ERR("not a function: %s", lake_repr(fnVal));
    }
    return result;
}
