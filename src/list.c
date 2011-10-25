/** 
  * list.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "int.h"
#include "lake.h"
#include "list.h"
#include "str.h"

/* TODO: use a linked list instead of this cheesy structure */

#define LIST_INIT_CAP 2

static LakeList *list_alloc(void)
{
    LakeList *list = malloc(sizeof(LakeList));
    VAL(list)->type = TYPE_LIST;
    VAL(list)->size = sizeof(LakeList);
    return list;
}

void list_free(LakeList *list)
{
  /* TODO: proper memory management ... refcounting? */
  if (list) {
    free(list);
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
    if (lk_is_type(TYPE_LIST, cdr)) {
        list = LIST(cdr);
        list_unshift(list, car);
    }
    else {
        list = list_make_with_capacity(2);
        list_append(list, car);
        list_append(list, cdr);
    }
    return list;
}

LakeList *list_make_with_capacity(size_t cap)
{
    LakeList *list = list_alloc();
    list->cap = cap;
    list->n = 0;
    list->vals = malloc(cap * sizeof(LakeVal *));
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
    list->vals = realloc(list->vals, list->cap * sizeof(LakeVal *));
    if (!list->vals) OOM();
}

LakeVal *list_set(LakeList *list, size_t i, LakeVal *val)
{
    if (i < list->n) {
        list->vals[i] = val;
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
    do {
      list->vals[i] = list->vals[i - 1];
        } while (i--);
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

bool list_equal(LakeList *a, LakeList *b)
{
    if (a == b) return TRUE;
    size_t n = LIST_N(a);
    if (n != LIST_N(b)) return FALSE;
    size_t i;
    for (i = 0; i < n; ++i) {
        if (!lake_equal(LIST_VAL(a, i), LIST_VAL(b, i))) return FALSE;
    }
    return TRUE;
}

LakeStr *list_to_str(LakeList *list)
{
  char *s = list_repr(list);
  LakeStr *str = lk_str_from_c(s);
  free(s);
  return str;
}

char *list_repr(LakeList *list)
{
  char *s = malloc(2);
  s[0] = '(';
  s[1] = '\0';
  int i;
  char *s2;
  LakeVal *val;
  for (i = 0; i < LIST_N(list); ++i) {
    val = LIST_VAL(list, i);
    if (val == VAL(list)) {
      s2 = strdup("[Circular]");
    }
    else {
      s2 = lake_repr(val);
    }
    s = lk_str_append(s, s2);
    free(s2);
    if (i != LIST_N(list) - 1) s = lk_str_append(s, " ");
  }
  return lk_str_append(s, ")");
}
