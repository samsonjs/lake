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
#include "minunit.h"
#include "lake.h"
#include "list.h"

int tests_run;
static LakeList *head;
static LakeVal *tail;
static LakeDottedList *dlist;
static char *REPR = "(() . ())";

/* LakeDottedList *dlist_make(LakeList *head, LakeVal *tail) */
static char *test_dlist_make(void)
{
    mu_assert("type is not TYPE_DLIST", IS(TYPE_DLIST, dlist));
    mu_assert("value size is incorrect", VAL_SIZE(dlist) == sizeof(LakeDottedList));
    mu_assert("head value is incorrect",
              lake_equal(VAL(head), VAL(DLIST_HEAD(dlist))));
    mu_assert("tail value is incorrect", lake_equal(tail, DLIST_TAIL(dlist)));
    return 0;
}

/* char *dlist_repr(LakeDottedList *dlist) */
static char *test_dlist_repr(void)
{
    mu_assert("dlist_repr is incorrect", strncmp(dlist_repr(dlist), REPR, strlen(REPR)) == 0);

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
        LakeDottedList *dlist2 = dlist_make(list, s_spam);
    mu_assert("", strncmp(dlist_repr(dlist2), REPR2, strlen(REPR2)) == 0);
    
    return 0;
}

/* gboolean dlist_equal(LakeDottedList *a, LakeDottedList *b) */
static char *test_dlist_equal(void)
{
    LakeDottedList *a = dlist;
    LakeDottedList *b = dlist_make(head, tail);
    LakeVal *null = VAL(list_make());
    LakeList *null_pair = list_cons(null, null);
    LakeDottedList *diff_head = dlist_make(null_pair, tail);
    LakeDottedList *diff_tail = dlist_make(head, VAL(null_pair));
    mu_assert("dlist a != a", dlist_equal(a, a));
    mu_assert("dlist a != b", dlist_equal(a, b));
    mu_assert("dlist a == diff_head", !dlist_equal(a, diff_head));
    mu_assert("dlist a == diff_tail", !dlist_equal(a, diff_tail));
    return 0;
}

static void setup(void)
{
    head = list_make();
    tail = VAL(list_make());
    dlist = dlist_make(head, tail);
}

static char *all_tests() {
    setup();
    mu_run_test(test_dlist_make);
    mu_run_test(test_dlist_repr);
    mu_run_test(test_dlist_equal);
    return 0;
}

int main(int argc, char const *argv[]) {
    char *result = all_tests();
    int pass = result == 0;
    printf("-- Dotted Lists --\n");
    if (pass) {
        printf("PASS: %d test%s run\n", tests_run, tests_run == 1 ? "" : "s");
    }
    else {
        printf("%s\n", result);
        printf("FAIL: %d test%s run\n", tests_run, tests_run == 1 ? "" : "s");
    }

    return !pass;
}

