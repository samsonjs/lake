#include <stdlib.h>
#include <string.h>
#include "int.h"
#include "lake.h"
#include "string.h"

static LakeStr *str_alloc(void)
{
    LakeStr *str = malloc(sizeof(LakeStr));
    str->base.type = TYPE_STR;
    str->base.size = sizeof(LakeStr);
    return str;
}

void str_free(LakeStr *str)
{
    free(str->s);
    free(str);
}

NILP str_set(LakeStr *str, char *s)
{
    str->n = strlen(s);
    str->s = strdup(s);
    return NIL;
}

LakeStr *str_from_c(char *s)
{
    LakeStr *str = str_alloc();
    str_set(str, s);
    return str;
}

LakeStr *str_make(void)
{
    return str_from_c("");
}

LakeInt *str_len(LakeStr *str)
{
    return int_from_c(str->n);
}

LakeStr *str_copy(LakeStr *str)
{
    return str_from_c(str->s);
}

char *str_val(LakeStr *str)
{
    return strdup(str->s);
}

#define MIN(a, b) (a < b ? a : b)

LakeInt *str_cmp(LakeStr *a, LakeStr *b)
{
    return int_from_c(strncmp(a->s, b->s, MIN(a->n, b->n)));
}

LakeSym *str_eq(LakeStr *a, LakeStr *b)
{
    return bool_from_int(strncmp(a->s, b->s, MIN(a->n, b->n)) == 0);
}

LakeStr *str_to_str(LakeStr *str)
{
    return str_copy(str);
}
