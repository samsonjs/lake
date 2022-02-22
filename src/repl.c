/**
 * repl.c
 * Lake Scheme
 *
 * Copyright 2011 Sami Samhuri
 * MIT License
 *
 * A quick and dirty scheme written in C, for fun and to use while
 * reading The Little Schemer.
 *
 */

#include "common.h"
#include "env.h"
#include "eval.h"
#include "lake.h"
#include "list.h"
#include "parse.h"
#include "str.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>

void print(LakeVal *expr) { printf("%s\n", lake_repr(expr)); }

static char first_char(char *s)
{
    char c;
    while ((c = *s++) && (c == ' ' || c == '\n' || c == '\t'))
        ;
    return c;
}

static LakeVal *prompt_read(LakeCtx *ctx, Env *env, char *prompt)
{
    static int n = 1024;
    printf("%s", prompt);
    char buf[n];
    if (!fgets(buf, n, stdin))
    {
        if (ferror(stdin))
        {
            fprintf(stderr, "error: cannot read from stdin");
        }
        if (feof(stdin))
        {
            return VAL(EOF);
        }
        return NULL;
    }
    /* trim the newline if any */
    buf[strcspn(buf, "\n")] = '\0';

    /* parse list expressions */
    if (first_char(buf) == '(')
    {
        return parse_expr(ctx, buf, strlen(buf));
    }

    /* try to parse a naked call without parens
    (makes the repl more palatable) */
    LakeList *list = parse_naked_list(ctx, buf, strlen(buf));
    if (!list || LIST_N(list) == 0) return NULL;

    LakeVal *result;

    /* naked call */
    LakeVal *head;
    if (is_special_form(ctx, list) ||
        (LIST_N(list) > 1 && (head = eval(ctx, env, LIST_VAL(list, 0))) &&
         CALLABLE(head)))
    {
        result = VAL(list);
    }

    /* probably not function calls, just give the first expr
    (maybe do an implicit progn thing here) */
    else
    {
        result = LIST_VAL(list, 0);
    }

    return result;
}

static void run_repl(LakeCtx *ctx, Env *env)
{
    puts("Lake Scheme v" LAKE_VERSION);
    LakeVal *expr;
    LakeVal *result;
    for (;;)
    {
        expr = prompt_read(ctx, env, "> ");
        if (expr == VAL(EOF)) break;
        if (expr == VAL(PARSE_ERR))
        {
            ERR("parse error");
            continue;
        }
        if (expr)
        {
            result = eval(ctx, env, expr);
            if (result) print(result);
        }
    }
}

static char *read_file(char const *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp)
    {
        size_t size = 4096;
        char buf[size];
        size_t n = size;
        size_t i = 0;
        size_t read;
        char *s = malloc(n);

        while (!feof(fp) && !ferror(fp))
        {
            read = fread(buf, 1, size, fp);
            if (i + read > n)
            {
                n += size;
                if (!(s = realloc(s, n))) OOM();
            }
            memcpy(s + i, buf, read);
            i += read;
        }
        s[i] = '\0';
        if (ferror(fp))
        {
            ERR("failed to read file %s: %s", filename, strerror(errno));
            return NULL;
        }
        fclose(fp);

        return s;
    }
    else
    {
        ERR("cannot open file %s: %s", filename, strerror(errno));
        return NULL;
    }
}

int main(int argc, char const *argv[])
{
    /* create an execution context */
    LakeCtx *ctx = lake_init();

    /* create and bind args */
    LakeVal **argVals = malloc(argc * sizeof(LakeVal *));
    int i;
    for (i = 0; i < argc; ++i)
    {
        argVals[i] = VAL(lake_str_from_c((char *)argv[i]));
    }
    LakeList *args = list_from_array(argc, argVals);
    free(argVals);
    env_define(ctx->toplevel, sym_intern(ctx, "args"), VAL(args));

    /* if a filename is given load the file */
    if (argc > 1)
    {
        char *text = read_file(argv[1]);
        if (text)
        {
            LakeList *exprs = parse_exprs(ctx, text, strlen(text));
            if (exprs)
            {
                eval_exprs(ctx, ctx->toplevel, exprs);
            }
        }
    }

    /* run the repl */
    run_repl(ctx, ctx->toplevel);

    return 0;
}
