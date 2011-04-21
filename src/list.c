/** 
  * list.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "int.h"
#include "lake.h"
#include "list.h"
#include "string.h"

/* TODO: use a linked list instead of this cheesy structure */

#define LIST_INIT_CAP 2

static LakeList *list_alloc(void)
{
    LakeList *list = g_malloc(sizeof(LakeList));
    VAL(list)->type = TYPE_LIST;
    VAL(list)->size = sizeof(LakeList);
    return list;
}

void list_free(LakeList *list)
{
	/* TODO: proper memory management ... refcounting? */
	if (list) {
		g_free(list);
	}
}

LakeList *list_make(void)
{
    LakeList *list = list_make_with_capacity(LIST_INIT_CAP);
    memset(list->vals, 0, list->cap);
    return list;
}

LakeList *list_cons(LakeVal *car, LakeVal *cdr)
{
    LakeList *list;
    if (IS(TYPE_LIST, cdr)) {
        list = LIST(cdr);
        list_unshift(list, car);
    }
    else {
        list = list_make_with_capacity(2);
        list->vals[0] = car;
        list->vals[1] = cdr;
    }
    return list;
}

LakeList *list_make_with_capacity(size_t cap)
{
    LakeList *list = list_alloc();
    list->cap = cap;
    list->n = 0;
    list->vals = g_malloc(cap * sizeof(LakeVal *));
    return list;
}

LakeList *list_from_array(size_t n, LakeVal *vals[])
{
    LakeList *list = list_make_with_capacity(n);
    memcpy(list->vals, vals, n * sizeof(LakeVal *));
    list->n = n;
    return list;
}

LakeInt *list_len(LakeList *list)
{
    return int_from_c(list->n);
}

LakeList *list_copy(LakeList *list)
{
    return list_from_array(list->n, list->vals);
}

static void list_grow(LakeList *list)
{
    list->cap *= 2;
    list->vals = g_realloc(list->vals, list->cap * sizeof(LakeVal *));
    if (!list->vals) OOM();
}

LakeVal *list_set(LakeList *list, size_t i, LakeVal *val)
{
    if (i >= 0 && i < list->n) {
        list->vals[i] = val;
    }
    else {
        ERR("list_set: index %zu is out of bounds (%zu)", i, list->n);
    }
    return NULL;
}

LakeVal *list_get(LakeList *list, LakeInt *li)
{
    int i = INT_VAL(li);
    if (i >= 0 && i < list->n) {
        return list->vals[i];
    }
    return NULL;
}

LakeVal *list_append(LakeList *list, LakeVal *val)
{
    if (list->n >= list->cap) {
        list_grow(list);
    }
    list->vals[list->n++] = val;
    return NULL;
}

LakeVal *list_shift(LakeList *list)
{
	LakeVal *head = NULL;
	if (list->n > 0) {
		head = list->vals[0];
		size_t i;
		size_t n = list->n;
		for (i = 1; i < n; ++i) {
			list->vals[i - 1] = list->vals[i];
		}
		list->n--;
	}
	return head;
}

LakeVal *list_unshift(LakeList *list, LakeVal *val)
{
	if (list->n == 0) {
        list_append(list, val);
    }
    else {
        if (list->n >= list->cap) {
            list_grow(list);
        }
		size_t i = list->n++;
		while (i--) {
			list->vals[i] = list->vals[i - 1];
		}
        list->vals[0] = val;
	}
	return NULL;
}

LakeVal *list_pop(LakeList *list)
{
	LakeVal *tail = NULL;
	if (list->n > 0) {
		tail = list->vals[list->n - 1];
		list->n--;
	}
	return tail;
}

LakeInt *list_cmp(LakeList *a, LakeList *b)
{
    /* TODO */
    return 0;
}

LakeBool *list_eq(LakeList *a, LakeList *b)
{
    /* TODO */
    return bool_from_int(a == b);
}

LakeStr *list_to_str(LakeList *list)
{
	char *s = list_repr(list);
	LakeStr *str = str_from_c(s);
	g_free(s);
    return str;
}

char *list_repr(LakeList *list)
{
	GString *s = g_string_new("(");
    int i;
	char *s2;
    for (i = 0; i < LIST_N(list); ++i) {
		s2 = repr(LIST_VAL(list, i));
		g_string_append(s, s2);
		g_free(s2);
		if (i != LIST_N(list) - 1) g_string_append(s, " ");
    }
	g_string_append(s, ")");
	gchar *repr = s->str;
	g_string_free(s, FALSE); /* don't free char data */
	return repr;
}
