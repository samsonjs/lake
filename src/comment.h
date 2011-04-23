/** 
  * comment.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_COMMENT_H
#define _LAKE_COMMENT_H 1

#include <glib.h>
#include "lake.h"

LakeComment *comment_make(LakeStr *text);
LakeComment *comment_from_c(char *text);
char *comment_repr(LakeComment *comment);
gboolean comment_equal(LakeComment *a, LakeComment *b);

#endif
