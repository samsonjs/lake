/** 
  * dlist.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_DLIST_H
#define _LAKE_DLIST_H 1

#include "lake.h"

LakeDottedList *dlist_make(LakeList *head, LakeVal *tail);
char *dlist_repr(LakeDottedList *dlist);

#endif
