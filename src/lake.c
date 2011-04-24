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
#include "str.h"
#include "symtable.h"

char *type_name(LakeVal *expr)
{
    static char *type_names[9] = { "nil", "symbol", "boolean", "integer", "string", "list",
                                   "dotted-list", "primitive", "function"
                                 };

    return type_names[expr->type];
}

void print(LakeVal *expr)
{
    printf("%s\n", repr(expr));
}

static char first_char(char *s)
{
    char c;
    while ((c = *s++) && (c == ' ' || c == '\n' || c == '\t'));
    return c;
}

static LakeVal *prompt_read(LakeCtx *ctx, Env *env, char *prompt)
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
	
	/* parse list expressions */
	if (first_char(buf) == '(') {
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
        (LIST_N(list) > 1 && (head = eval(ctx, env, LIST_VAL(list, 0))) && CALLABLE(head))) {
        result = VAL(list);
    }

    /* probably not function calls, just give the first expr
       (maybe do an implicit progn thing here) */
    else {
        result = LIST_VAL(list, 0);
    }
    
    return result;
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
        s = BOOL_REPR(BOOL(expr));
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

gboolean lake_is(LakeVal *a, LakeVal *b)
{
    if (IS(TYPE_INT, a) && IS(TYPE_INT, b)) {
        return INT_VAL(INT(a)) == INT_VAL(INT(b));
    }
    if (IS_NIL(a) && IS_NIL(b)) return TRUE;
    return a == b;
}

gboolean lake_equal(LakeVal *a, LakeVal *b)
{
    if (a->type != b->type) return FALSE;
    switch (a->type) {

        /* singletons can be compared directly */
        case TYPE_SYM:
        case TYPE_BOOL:
        case TYPE_PRIM:
        case TYPE_FN:
            return a == b;
        
        case TYPE_INT:
            return INT_VAL(INT(a)) == INT_VAL(INT(b));
        
        case TYPE_STR:
            return str_equal(STR(a), STR(b));
        
        case TYPE_LIST:
            return list_equal(LIST(a), LIST(b));
        
        case TYPE_DLIST:
            return dlist_equal(DLIST(a), DLIST(b));
        
        case TYPE_COMM:
            return comment_equal(COMM(a), COMM(b));
        
        default:
            ERR("unknown type %d (%s)", a->type, type_name(a));
            return FALSE;
    }
}

static void run_repl(LakeCtx *ctx, Env *env)
{
    puts("Lake Scheme v" LAKE_VERSION);
    LakeVal *expr;
    LakeVal *result;
    for (;;) {
        expr = prompt_read(ctx, env, "> ");
        if (expr == VAL(EOF)) break;
        if (expr == VAL(PARSE_ERR)) {
            ERR("parse error");
            continue;
        }
        if (expr) {
            result = eval(ctx, env, expr);
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

LakeBool *bool_make(gboolean val)
{
    LakeBool *b = g_malloc(sizeof(LakeBool));
    VAL(b)->type = TYPE_BOOL;
    VAL(b)->size = sizeof(LakeBool);
    b->val = val;
    return b;
}

LakeCtx *lake_init(void)
{
    LakeCtx *ctx = g_malloc(sizeof(LakeCtx));
    ctx->toplevel = env_make(NULL);
    ctx->symbols = g_hash_table_new(g_str_hash, g_str_equal);
    ctx->special_form_handlers = symtable_make();
    ctx->T = bool_make(TRUE);
    ctx->F = bool_make(FALSE);
    return ctx;
}

int main (int argc, char const *argv[])
{
    /* create an execution context */
    LakeCtx *ctx = lake_init();
    bind_primitives(ctx);
    init_special_form_handlers(ctx);

    /* create and bind args */
    LakeVal **argVals = g_malloc(argc * sizeof(LakeVal *));
    int i;
    for (i = 0; i < argc; ++i) {
        argVals[i] = VAL(str_from_c((char *)argv[i]));
    }
    LakeList *args = list_from_array(argc, argVals);
    free(argVals);
    env_define(ctx->toplevel, sym_intern(ctx, "args"), VAL(args));
    
    /* if a filename is given load the file */
    if (argc > 1) {
        char *text = read_file(argv[1]);
        if (text) {
            LakeList *exprs = parse_exprs(ctx, text, strlen(text));
            if (exprs) {
                eval_exprs(ctx, ctx->toplevel, exprs);
            }
        }
    }
    
    /* run the repl */
    run_repl(ctx, ctx->toplevel);
    
    return 0;
}
