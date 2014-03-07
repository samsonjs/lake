/**
  * test_lake.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <string.h>
#include "laketest.h"
#include "lake.h"
#include "list.h"

void setup(void);
static char *test_list_make(void);
static char *test_list_cons(void);
static char *test_list_make_with_capacity(void);
static char *test_list_from_array(void);
static char *test_list_copy(void);
static char *test_list_set(void);
static char *test_list_append(void);
static char *test_list_get(void);
static char *test_list_len(void);
static char *test_list_pop(void);
static char *test_list_shift(void);
static char *test_list_unshift(void);
static char *test_list_equal(void);
static char *test_list_to_str(void);
static char *test_list_repr(void);

int main(int argc, char const *argv[])
{
    setup();
    return !lt_run_tests("List", (test_fn[]){
        test_list_make,
        test_list_cons,
        test_list_make_with_capacity,
        test_list_from_array,
        test_list_copy,
        test_list_set,
        test_list_append,
        test_list_get,
        test_list_len,
        test_list_pop,
        test_list_shift,
        test_list_unshift,
        test_list_equal,
        test_list_to_str,
        test_list_repr,
        NULL
    });
}

void setup(void)
{
    // TODO
}

/* LakeList *list_make(void) */
static char *test_list_make(void)
{
    return 0;
}

/* LakeList *list_cons(LakeVal *car, LakeVal *cdr) */
static char *test_list_cons(void)
{
    return 0;
}

/* LakeList *list_make_with_capacity(size_t cap) */
static char *test_list_make_with_capacity(void)
{
    return 0;
}

/* LakeList *list_from_array(size_t n, LakeVal *vals[]) */
static char *test_list_from_array(void)
{
    return 0;
}

/* LakeList *list_copy(LakeList *list) */
static char *test_list_copy(void)
{
    return 0;
}

/* LakeVal *list_set(LakeList *list, size_t i, LakeVal *val) */
static char *test_list_set(void)
{
    return 0;
}

/* LakeVal *list_append(LakeList *list, LakeVal *val) */
static char *test_list_append(void)
{
    return 0;
}

/* LakeVal *list_get(LakeList *list, LakeInt *li) */
static char *test_list_get(void)
{
    return 0;
}

/* LakeInt *list_len(LakeList *list) */
static char *test_list_len(void)
{
    return 0;
}

/* LakeVal *list_pop(LakeList *list) */
static char *test_list_pop(void)
{
    return 0;
}

/* LakeVal *list_shift(LakeList *list) */
static char *test_list_shift(void)
{
    return 0;
}

/* LakeVal *list_unshift(LakeList *list, LakeVal *val) */
static char *test_list_unshift(void)
{
    return 0;
}

/* int list_equal(LakeList *a, LakeList *b) */
static char *test_list_equal(void)
{
    return 0;
}

/* LakeStr *list_to_str(LakeList *list) */
static char *test_list_to_str(void)
{
    return 0;
}

/* char *list_repr(LakeList *list) */
static char *test_list_repr(void)
{
    return 0;
}
