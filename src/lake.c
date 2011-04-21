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

#include <errno.h>
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include "comment.h"
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

char *type_name(LakeVal *expr)
{
    static char *type_names[9] = { "nil", "symbol", "boolean", "integer", "string", "list",
                                   "dotted-list", "primitive", "function"
                                 };

    return type_names[expr->type];
}

void print(LakeVal *expr)
{
    /* printf("[%s]\n", type_name(expr)); */
    printf("%s\n", repr(expr));
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
    
    case TYPE_PRIM:
        s = prim_repr(PRIM(expr));
        break;
    
    case TYPE_FN:
        s = fn_repr(FN(expr));
        break;
    
    case TYPE_COMM:
        s = comment_repr(COMM(expr));
        break;
    
    default:
        printf("error: unrecognized value, type %d, size %zu bytes", expr->type, expr->size);
        s = g_strdup("");
    }
    
    return s;
}

static void run_repl(Env *env)
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

char *read_file(char const *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp) {
        size_t size = 4096;
        char buf[size];
        size_t n = size;
        size_t i = 0;
        size_t read;
        char *s = g_malloc(n);
        
        while (!feof(fp) && !ferror(fp)) {
            read = fread(buf, 1, size, fp);
            if (i + read > n) {
                n += size;
                if (!(s = g_realloc(s, n))) OOM();
            }
            memcpy(s + i, buf, read);
            i += read;
        }
        s[i] = '\0';
        if (ferror(fp)) {
            ERR("failed to read file %s: %s", filename, strerror(errno));
            return NULL;
        }
        fclose(fp);
        
        return s;
    }
    else {
        ERR("cannot open file %s: %s", filename, strerror(errno));
        return NULL;
    }
}

int main (int argc, char const *argv[])
{
    /* create a top level environment */
    Env *env = primitive_bindings();

    /* create and bind args */
    LakeVal **argVals = g_malloc(argc * sizeof(LakeVal *));
    int i;
    for (i = 0; i < argc; ++i) {
        argVals[i] = VAL(str_from_c((char *)argv[i]));
    }
    LakeList *args = list_from_array(argc, argVals);
    free(argVals);
    env_define(env, sym_intern("args"), VAL(args));
    
    /* if a filename is given load the file */
    if (argc > 1) {
        char *text = read_file(argv[1]);
        if (text) {
            LakeList *exprs = parse_exprs(text, strlen(text));
            if (exprs) {
                eval_exprs(env, exprs);
            }
        }
    }
    
    /* run the repl */
    run_repl(env);
    
    return 0;
}
