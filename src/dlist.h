/** 
  * dlist.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_DLIST_H
#define _LAKE_DLIST_H

#include "common.h"
#include "lake.h"

LakeDottedList *dlist_make(LakeList *head, LakeVal *tail);
LakeList *dlist_head(LakeDottedList *dlist);
LakeVal *dlist_tail(LakeDottedList *dlist);
char *dlist_repr(LakeDottedList *dlist);
bool dlist_equal(LakeDottedList *a, LakeDottedList *b);

#endif
