/** 
  * parse.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dlist.h"
#include "int.h"
#include "lake.h"
#include "list.h"
#include "parse.h"
#include "string.h"
#include "sym.h"

struct context {
    char *s;
    size_t n;
    size_t i;
};
typedef struct context Ctx;

static LakeVal *_parse_expr(Ctx *ctx);

LakeVal *parse_expr(char *s, size_t n)
{
    Ctx ctx = { s, n, 0 };
    LakeVal *result = _parse_expr(&ctx);
    if (ctx.i < ctx.n) {
        char *trailing = ctx.s + ctx.i;
        printf("warning: ignoring %d trailing chars: %s\n", (int)(ctx.n - ctx.i), trailing);
    }
    return result;
}

static char peek(Ctx *ctx)
{
    if (ctx->i < ctx->n) return ctx->s[ctx->i];
    return PARSE_EOF;
}

static void consume(Ctx *ctx, size_t n)
{
    if (ctx->i + n > ctx->n) {
        die("ERROR: cannot consume, no more input");
    }
    ctx->i += n;
}

static char consume1(Ctx *ctx)
{
    char c = peek(ctx);
    consume(ctx, 1);
    return c;
}

static char ch(Ctx *ctx, char expected)
{
    char c = peek(ctx);
    if (c == expected) {
        consume1(ctx);
        return c;
    }
    char *msg = g_malloc(64);
    sprintf(msg, "parse error, expected '%c' got '%c'", expected, c);
    die(msg);
    return 0; /* placate the compiler */
}

static int maybe_spaces(Ctx *ctx)
{
    char *p;
    while ((p = strchr(" \r\n\t", peek(ctx))) != NULL) {
        consume1(ctx);
    }
    return 1;
}
/*
static int whitespace(Ctx *ctx)
{
    int result = 0;
    char *p;
    while ((p = strchr(" \r\n\t", peek(ctx))) != NULL) {
        consume1(ctx);
        result = 1;
    }
    return result;
}
*/
static gboolean is_letter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static gboolean is_symbol(char c)
{
    return strchr("!$%&|*+-/:<=>?@^_~#", c) != NULL;
}

static gboolean is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static char *parse_while(Ctx *ctx, gboolean (*is_valid)(char))
{
    size_t n = 8;
    size_t i = 0;
    char *s = g_malloc(n);
    char c;
    while ((c = peek(ctx)) != PARSE_EOF && is_valid(c)) {
        s[i++] = c;
        consume1(ctx);
        /* grow if necessary */
        if (i >= n) {
            n *= 2;
            char *t = g_realloc(s, n);
            if (!t) oom();
            s = t;
        }
    }
    s[i] = '\0';
    return s;
}

static LakeVal *parse_int(Ctx *ctx)
{
    int n = 0;
    char c = peek(ctx);
    char sign = c == '-' ? -1 : 1;
    if (c == '-' || c == '+') {
        consume1(ctx);
    }
    while (is_digit(c = peek(ctx))) {
        n *= 10;
        n += c - '0';
        consume1(ctx);
    }
    return VAL(int_from_c(sign * n));
}

static gboolean is_sym_char(char c)
{
    return is_letter(c) || is_symbol(c) || is_digit(c);
}

static LakeVal *parse_sym(Ctx *ctx)
{
	LakeVal *val;
    static int size = 1024;
    char s[size];
    char c;
    int i = 0;
    while (is_sym_char(c = peek(ctx)) && i < size - 1) {
        s[i++] = c;
        consume1(ctx);
    }
    s[i] = '\0';
	if (g_strcmp0(s, "#t") == 0) {
		val = VAL(T);
	} else if (g_strcmp0(s, "#f") == 0) {
		val = VAL(F);
	} else {
		val = VAL(sym_intern(s));
	}
    return val;
}

static char escape_char(char c)
{
    switch (c) {

    case 'n':
        c = '\n';
        break;

    case 'r':
        c = '\r';
        break;
        
    case 't':
        c = '\t';
        break;

    default:
        /* noop */
        break;
        
    }
    return c;
}

static LakeVal *parse_str(Ctx *ctx)
{
    size_t n = 8;
    size_t i = 0;
    char *s = g_malloc(n);
    char c;
    ch(ctx, '"');
    while ((c = peek(ctx)) != PARSE_EOF && c != '"') {
        /* handle backslash escapes */
        if (c == '\\') {
            consume1(ctx);
            c = escape_char(peek(ctx));
            if (c == PARSE_EOF) break;
        }
        s[i++] = c;
        consume1(ctx);
        /* grow if necessary */
        if (i >= n) {
            n *= 2;
            char *t = g_realloc(s, n);
            if (!t) oom();
            s = t;
        }
    }
    s[i] = '\0';
    ch(ctx, '"');
    LakeStr *str = str_from_c(s);
    g_free(s);
    return VAL(str);
}

static LakeVal* parse_list(Ctx *ctx)
{
    LakeList *list = list_make();
    ch(ctx, '(');
    char c;
    while ((c = peek(ctx)) != ')') {
        if (c == PARSE_EOF) {
            printf("error: end of input while parsing list");
            list_free(list);
            ctx-> i = ctx->n;
            return NULL;
        }
        
        /* check for dotted lists */
        if (c == '.') {
            ch(ctx, '.');
            maybe_spaces(ctx);
            LakeVal *tail = _parse_expr(ctx);
            if (tail == VAL(PARSE_ERR)) {
                list_free(list);
                ctx->i = ctx->n;
                return NULL;
            }
            ch(ctx, ')');
            return VAL(dlist_make(list, tail));
        }
        
        LakeVal *val = _parse_expr(ctx);
        if (val == VAL(PARSE_ERR)) {
            list_free(list);
            ctx->i = ctx->n;
            return NULL;
        }
        list_append(list, val);
    }
    ch(ctx, ')');
    return VAL(list);
}

static gboolean is_not_newline(char c)
{
    return !(c == '\n' || c == '\r');
}

static void parse_comment(Ctx *ctx)
{
    g_free(parse_while(ctx, is_not_newline));
}

static LakeVal *_parse_expr(Ctx *ctx)
{
    LakeVal *result = NULL;
    char c = peek(ctx);
    if (c >= '0' && c <= '9') {
        result = VAL(parse_int(ctx));
    }
    else if (is_letter(c) || is_symbol(c)) {
        result = parse_sym(ctx);
    }
    else if (c == '"') {
        result = parse_str(ctx);
    }
    /* TODO: quote
    else if (c == '\'') {
        result = parse_quoted(ctx);
    }
    */
    else if (c == '(') {
        result = parse_list(ctx);
    }
    else if (c == ';') {
        parse_comment(ctx);
    }
    else if (c == PARSE_EOF) {
        /* noop */
    }
    else {
        char *msg = g_strdup_printf("unexpected char '%c'", c);
        err(msg);
        g_free(msg);
        result = VAL(PARSE_ERR);
        ctx->i = ctx->n; /* consume the rest */
    }
    maybe_spaces(ctx);
    return result;
}
