/** 
  * test_comment.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <string.h>
#include "minunit.h"
#include "comment.h"
#include "lake.h"
#include "str.h"

#define TEXT "you are not expected to understand this"

int tests_run;
static LakeStr *text = NULL;

/* LakeComment *comment_make(LakeStr *text) */
static char *test_comment_make(void)
{
    LakeComment *comment = comment_make(text);
    mu_assert("type is not TYPE_COMM", IS(TYPE_COMM, comment));
    mu_assert("value size is incorrect", VAL_SIZE(comment) == sizeof(LakeComment));
    mu_assert("comment text is incorrect", str_equal(text, COMM_TEXT(comment)));
    return 0;
}

/* LakeComment *comment_from_c(char *text) */
static char *test_comment_from_c(void)
{
    LakeComment *comment = comment_from_c(TEXT);
    mu_assert("type is not TYPE_COMM", IS(TYPE_COMM, comment));
    mu_assert("value size is incorrect", VAL_SIZE(comment) == sizeof(LakeComment));
    mu_assert("comment text is incorrect", str_equal(text, COMM_TEXT(comment)));
    return 0;
}

/* char *comment_repr(LakeComment *comment) */
static char *test_comment_repr(void)
{
    LakeComment *comment = comment_make(text);
    mu_assert("comment_repr is incorrect", strncmp(comment_repr(comment), TEXT, strlen(TEXT)) == 0);
    return 0;
}

/* gboolean comment_equal(LakeComment *a, LakeComment *b) */
static char *test_comment_equal(void)
{
    LakeComment *a = comment_make(text);
    LakeComment *b = comment_from_c(TEXT);
    LakeComment *c = comment_from_c("and now for something completely different");
    mu_assert("comment a != a", comment_equal(a, a));
    mu_assert("comment a != b", comment_equal(a, b));
    mu_assert("comment a == c", !comment_equal(a, c));
    mu_assert("comment b == c", !comment_equal(b, c));
    return 0;
}

static void setup(void)
{
    text = str_from_c(TEXT);
}

static char *all_tests() {
    setup();
    mu_run_test(test_comment_make);
    mu_run_test(test_comment_from_c);
    mu_run_test(test_comment_repr);
    mu_run_test(test_comment_equal);
    return 0;
}

int main(int argc, char const *argv[]) {
    char *result = all_tests();
    int pass = result == 0;
    printf("-- Comments --\n");
    if (pass) {
        printf("PASS: %d test%s run\n", tests_run, tests_run == 1 ? "" : "s");
    }
    else {
        printf("%s\n", result);
        printf("FAIL: %d test%s run\n", tests_run, tests_run == 1 ? "" : "s");
    }

    return !pass;
}
