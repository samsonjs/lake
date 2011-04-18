#ifndef _LIST_H
#define _LIST_H 1

#include <stdlib.h>
#include "lake.h"
#include "string.h"

LakeList *list_make(void);
LakeList *list_make_with_cap(size_t cap);
LakeList *list_from_array(size_t n, LakeVal *vals[]);
LakeInt *list_len(LakeList *list);
NILP list_grow(LakeList *list);
NILP list_append(LakeList *list, LakeVal *val);
NILP list_set(LakeList *list, size_t i, LakeVal *val);
LakeVal *list_get(LakeList *list, LakeInt *li);
LakeInt *list_cmp(LakeList *a, LakeList *b);
LakeSym *list_eq(LakeList *a, LakeList *b);
LakeStr *list_to_str(LakeList *list);

#endif
