/** 
  * comment.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include "comment.h"
#include "lake.h"
#include "string.h"

static LakeComment *comment_alloc(void)
{
	LakeComment *comment = g_malloc(sizeof(LakeComment));
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
    return comment_make(str_from_c(text));
}

char *comment_repr(LakeComment *comment)
{
	return g_strdup(STR_S(comment->text));
}

gboolean comm_equal(LakeComment *a, LakeComment *b)
{
    return str_equal(COMM_TEXT(a), COMM_TEXT(b));
}