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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "env.h"
#include "lake.h"
#include "list.h"
#include "parse.h"
#include "string.h"

static LakeVal _NIL = { TYPE_NIL, sizeof(LakeVal) };
LakeVal *NIL = &_NIL;

void die(char *msg)
{
    printf("error: %s\n", msg);
    exit(1);
}

void oom(void)
{
    die("out of memory");
}

Env *primitive_bindings(void)
{
    Env *env = shared_env();
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
        return NULL;
    }
    return parse_expr(buf, strlen(buf));
}

void run_repl_with_env(Env *env)
{
    puts("Lake Scheme v" LAKE_VERSION);
    LakeVal *expr;
    LakeVal *result;
    for (;;) {
        expr = prompt_read("> ");
        if (!expr) return;
        result = env_eval(env, expr);
        print(result);
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
    char *s = NULL;
    LakeStr *str;
    
    switch (expr->type) {

    case TYPE_NIL:
        s = strdup("nil");
        break;

    case TYPE_SYM:
        s = sym_repr(SYM(expr));
        break;

    case TYPE_BOOL:
        printf("%s:%d TODO: repr for bools", __FILE__, __LINE__);
        s = strdup("[bool]");
        break;

    case TYPE_INT:
        str = int_to_str(INT(expr));
        s = str_val(str);
        str_free(str);
        break;

    case TYPE_STR:
        str = STR(expr);
        s = malloc(STR_N(str) + 3);
        sprintf(s, "\"%s\"", STR_S(str));
        break;

    case TYPE_LIST:
		s = list_repr(LIST(expr));
        break;
        
    default:
        printf("unrecognized value, type %d, size %Zu bytes", expr->type, expr->size);
        s = strdup("unrecognized value");
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
