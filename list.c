#include <stdlib.h>
#include "int.h"
#include "lake.h"
#include "list.h"
#include "string.h"

#define LIST_INIT_CAP 2

static LakeList *list_alloc(void)
{
    LakeList *list = malloc(sizeof(LakeList));
    list->base.type = TYPE_LIST;
    list->base.size = sizeof(LakeList);
    return list;
}

LakeList *list_make(void)
{
    LakeList *list = list_make_with_cap(LIST_INIT_CAP);
    int i;
    for (i = 0; i < list->cap; ++i) {
        list->vals[i] = NULL;
    }
    return list;
}

LakeList *list_make_with_cap(size_t cap)
{
    LakeList *list = list_alloc();
    list->cap = cap;
    list->n = 0;
    list->vals = malloc(cap * sizeof(LakeVal *));
    return list;
}

LakeList *list_from_array(size_t n, LakeVal *vals[])
{
    LakeList *list = list_make_with_cap(n);
    int i;
    for (i = 0; i < n; ++i) {
        list->vals[i] = vals[i];
    }
    return list;
}

LakeInt *list_len(LakeList *list)
{
    return int_from_c(list->n);
}

NILP list_grow(LakeList *list)
{
    list->cap *= 2;
    LakeVal **new_vals;
    new_vals = malloc(list->cap * sizeof(LakeVal *));
    int i;
    for (i = 0; i < list->n; ++i) {
        new_vals[i] = list->vals[i];
    }
    free(list->vals);
    list->vals = new_vals;
    return NIL;
}

NILP list_append(LakeList *list, LakeVal *val)
{
    if (list->n >= list->cap) {
        list_grow(list);
    }
    list->vals[list->n++] = val;
    return NIL;
}

NILP list_set(LakeList *list, size_t i, LakeVal *val)
{
    if (i < 0 || i >= list->n) {
        /* TODO error */
        return NULL;
    }
    list->vals[i] = val;
    return NIL;
}

LakeVal *list_get(LakeList *list, LakeInt *li)
{
    int i = int_val(li);
    if (i < 0 || i >= list->n) {
        return NIL;
    }
    return VAL_OR_NIL(list->vals[i]);
}

LakeInt *list_cmp(LakeList *a, LakeList *b)
{
    /* TODO */
    return 0;
}

LakeSym *list_eq(LakeList *a, LakeList *b)
{
    /* TODO */
    return bool_from_int(a == b);
}

LakeStr *list_to_str(LakeList *list)
{
    /* TODO */
    return str_from_c("[TODO]");
}
