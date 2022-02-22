/**
 * comment.c
 * Lake Scheme
 *
 * Copyright 2011 Sami Samhuri
 * MIT License
 *
 */

#include "comment.h"
#include "common.h"
#include "lake.h"
#include "str.h"
#include <string.h>

static LakeComment *comment_alloc(void)
{
    LakeComment *comment = malloc(sizeof(LakeComment));
    VAL(comment)->type = TYPE_COMM;
    VAL(comment)->size = sizeof(LakeComment);
    return comment;
}

LakeComment *comment_make(LakeStr *text)
{
    LakeComment *comment = comment_alloc();
    comment->text = text;
    return comment;
}

LakeComment *comment_from_c(char *text)
{
    return comment_make(lake_str_from_c(text));
}

char *comment_repr(LakeComment *comment)
{
    return strndup(STR_S(comment->text), STR_N(comment->text));
}

bool comment_equal(LakeComment *a, LakeComment *b)
{
    return lake_str_equal(COMM_TEXT(a), COMM_TEXT(b));
}
