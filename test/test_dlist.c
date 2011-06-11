/** 
  * test_dlist.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include <string.h>
#include "common.h"
#include "laketest.h"
#include "lake.h"
#include "list.h"

static LakeList *head;
static LakeVal *tail;
static LakeDottedList *dlist;
static char *REPR = "(() . ())";

/* LakeDottedList *dlist_make(LakeList *head, LakeVal *tail) */
static char *test_dlist_make(void)
{
    lt_assert("type is not TYPE_DLIST", lk_is_type(TYPE_DLIST, dlist));
    lt_assert("value size is incorrect", lk_val_size(dlist) == sizeof(LakeDottedList));
    lt_assert("head value is incorrect",
              lake_equal(VAL(head), VAL(dlist_head(dlist))));
    lt_assert("tail value is incorrect", lake_equal(tail, dlist_tail(dlist)));
    return 0;
}

/* char *dlist_repr(LakeDottedList *dlist) */
static char *test_dlist_repr(void)
{
    lt_assert("dlist_repr is incorrect", strncmp(dlist_repr(dlist), REPR, strlen(REPR)) == 0);

    char *REPR2 = "(spam eggs bacon spam eggs . spam)";
    LakeCtx *lake = lake_init();
    LakeSym *s_spam = sym_intern(lake, "spam");
    LakeSym *s_eggs = sym_intern(lake, "eggs");
    LakeSym *s_bacon = sym_intern(lake, "bacon");
    LakeList *list = list_make();
    list_append(list, VAL(s_spam));
    list_append(list, VAL(s_eggs));
    list_append(list, VAL(s_bacon));
    list_append(list, VAL(s_spam));
    list_append(list, VAL(s_eggs));
    LakeDottedList *dlist2 = dlist_make(list, VAL(s_spam));
    lt_assert("", strncmp(dlist_repr(dlist2), REPR2, strlen(REPR2)) == 0);
    
    return 0;
}

/* bool dlist_equal(LakeDottedList *a, LakeDottedList *b) */
static char *test_dlist_equal(void)
{
    LakeDottedList *a = dlist;
    LakeDottedList *b = dlist_make(head, tail);
    LakeVal *null = VAL(list_make());
    LakeList *null_pair = list_cons(null, null);
    LakeDottedList *diff_head = dlist_make(null_pair, tail);
    LakeDottedList *diff_tail = dlist_make(head, VAL(null_pair));
    lt_assert("dlist a != a", dlist_equal(a, a));
    lt_assert("dlist a != b", dlist_equal(a, b));
    lt_assert("dlist a == diff_head", !dlist_equal(a, diff_head));
    lt_assert("dlist a == diff_tail", !dlist_equal(a, diff_tail));
    return 0;
}

static void setup(void)
{
    head = list_make();
    tail = VAL(list_make());
    dlist = dlist_make(head, tail);
}

int main(int argc, char const *argv[])
{
    setup();
    return !lt_run_tests("Dotted Lists", (test_fn[]){
        test_dlist_make,
        test_dlist_repr,
        test_dlist_equal,
        NULL
    });
}

