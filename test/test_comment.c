/**
 * test_comment.c
 * Lake Scheme
 *
 * Copyright 2011 Sami Samhuri
 * MIT License
 *
 */

#include "comment.h"
#include "lake.h"
#include "laketest.h"
#include "str.h"
#include <string.h>

#define TEXT "you are not expected to understand this"

void setup(void);
static char *test_comment_make(void);
static char *test_comment_from_c(void);
static char *test_comment_equal(void);
static char *test_comment_repr(void);

static LakeStr *text = NULL;

int main(int argc, char const *argv[])
{
    setup();
    return !lt_run_tests(
        "Comments", (test_fn[]){test_comment_make, test_comment_from_c,
                                test_comment_repr, test_comment_equal, NULL});
}

void setup(void) { text = lake_str_from_c(TEXT); }

/* LakeComment *comment_make(LakeStr *text) */
static char *test_comment_make(void)
{
    LakeComment *comment = comment_make(text);
    lt_assert("type is not TYPE_COMM", lake_is_type(TYPE_COMM, comment));
    lt_assert("value size is incorrect",
              lake_val_size(comment) == sizeof(LakeComment));
    lt_assert("comment text is incorrect",
              lake_str_equal(text, COMM_TEXT(comment)));
    return 0;
}

/* LakeComment *comment_from_c(char *text) */
static char *test_comment_from_c(void)
{
    LakeComment *comment = comment_from_c(TEXT);
    lt_assert("type is not TYPE_COMM", lake_is_type(TYPE_COMM, comment));
    lt_assert("value size is incorrect",
              lake_val_size(comment) == sizeof(LakeComment));
    lt_assert("comment text is incorrect",
              lake_str_equal(text, COMM_TEXT(comment)));
    return 0;
}

/* char *comment_repr(LakeComment *comment) */
static char *test_comment_repr(void)
{
    LakeComment *comment = comment_make(text);
    lt_assert("comment_repr is incorrect",
              strncmp(comment_repr(comment), TEXT, strlen(TEXT)) == 0);
    return 0;
}

/* bool comment_equal(LakeComment *a, LakeComment *b) */
static char *test_comment_equal(void)
{
    LakeComment *a = comment_make(text);
    LakeComment *b = comment_from_c(TEXT);
    LakeComment *c =
        comment_from_c("and now for something completely different");
    lt_assert("comment a != a", comment_equal(a, a));
    lt_assert("comment a != b", comment_equal(a, b));
    lt_assert("comment a == c", !comment_equal(a, c));
    lt_assert("comment b == c", !comment_equal(b, c));
    return 0;
}
