/** 
  * dlist.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include "dlist.h"
#include "common.h"
#include "lake.h"

static LakeDottedList *dlist_alloc(void)
{
  LakeDottedList *dlist = malloc(sizeof(LakeDottedList));
  VAL(dlist)->type = TYPE_DLIST;
  VAL(dlist)->size = sizeof(LakeDottedList);
  return dlist;
}

LakeDottedList *dlist_make(LakeList *head, LakeVal *tail)
{
  LakeDottedList *dlist = dlist_alloc();
  dlist->head = head;
  dlist->tail = tail;
  return dlist;
}

LakeList *dlist_head(LakeDottedList *dlist)
{
  return dlist->head;
}

LakeVal *dlist_tail(LakeDottedList *dlist)
{
  return dlist->tail;
}

char *dlist_repr(LakeDottedList *dlist)
{
  char *s = malloc(2);
  s[0] = '(';
  s[1] = '\0';
  int i;
  char *s2;
  if (dlist->head && LIST_N(dlist->head)) {
    for (i = 0; i < LIST_N(dlist->head); ++i) {
      s2 = lake_repr(LIST_VAL(dlist->head, i));
      s = lk_str_append(s, s2);
      free(s2);
      if (i != LIST_N(dlist->head) - 1) s = lk_str_append(s, " ");
    }
  }
  else if (dlist->head) {
    s2 = lake_repr(dlist->head);
    s = lk_str_append(s, s2);
    free(s2);
  }
  s = lk_str_append(s, " . ");
  s2 = lake_repr(dlist->tail);
  s = lk_str_append(s, s2);
  free(s2);
  return lk_str_append(s, ")");
}

bool dlist_equal(LakeDottedList *a, LakeDottedList *b)
{
  LakeVal *headA = VAL(dlist_head(a));
  LakeVal *tailA = dlist_tail(a);
  LakeVal *headB = VAL(dlist_head(b));
  LakeVal *tailB = dlist_tail(b);
  return lake_equal(headA, headB) && lake_equal(tailA, tailB);
}
