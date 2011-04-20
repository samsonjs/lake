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
#include "string.h"

static LakeBool _T = { { TYPE_BOOL, sizeof(LakeBool) }, TRUE };
static LakeBool _F = { { TYPE_BOOL, sizeof(LakeBool) }, FALSE };
LakeBool *T = &_T;
LakeBool *F = &_F;

void die(char *msg)
{
	err(msg);
    exit(1);
}

void err(char *msg)
{
	printf("error: %s\n", msg);
}

void oom(void)
{
    die("out of memory");
}

Env *primitive_bindings(void)
{
    Env *env = env_toplevel();
    /* TODO */
    return env;
}

void print(LakeVal *val)
{
    puts(repr(val));
}

LakeVal *prompt_read(char *prompt)
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

void run_repl_with_env(Env *env)
{
    puts("Lake Scheme v" LAKE_VERSION);
    LakeVal *expr;
    LakeVal *result;
    for (;;) {
        expr = prompt_read("> ");
        if (expr == VAL(EOF)) break;
        if (expr == VAL(PARSE_ERR)) {
            err("parse error");
            continue;
        }
        if (expr) {
            result = eval(env, expr);
            if (result) print(result);
        }
    }
}

void run_repl(void)
{
    run_repl_with_env(primitive_bindings());
}

void run_one_then_repl(int argc, char const *args[])
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
