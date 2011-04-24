/** 
  * dlist.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include "dlist.h"
#include "lake.h"

static LakeDottedList *dlist_alloc(void)
{
	LakeDottedList *dlist = g_malloc(sizeof(LakeDottedList));
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

char *dlist_repr(LakeDottedList *dlist)
{
	GString *s = g_string_new("(");
    int i;
	char *s2;
	if (dlist->head) {
        for (i = 0; i < LIST_N(dlist->head); ++i) {
		    s2 = lake_repr(LIST_VAL(dlist->head, i));
		    g_string_append(s, s2);
		    g_free(s2);
		    if (i != LIST_N(dlist->head) - 1) g_string_append(s, " ");
        }
    }
	g_string_append(s, " . ");
    s2 = lake_repr(dlist->tail);
    g_string_append(s, s2);
    g_free(s2);
	g_string_append(s, ")");
	gchar *repr = s->str;
	g_string_free(s, FALSE); /* don't free char data */
	return repr;
}

gboolean dlist_equal(LakeDottedList *a, LakeDottedList *b)
{
    LakeVal *headA = VAL(DLIST_HEAD(a));
    LakeVal *tailA = DLIST_TAIL(a);
    LakeVal *headB = VAL(DLIST_HEAD(b));
    LakeVal *tailB = DLIST_TAIL(b);
    return lake_equal(headA, headB) && lake_equal(tailA, tailB);
}
