#ifndef _STRING_H
#define _STRING_H 1

#include "lake.h"

LakeStr *str_make(void);
void str_free(LakeStr *str);
LakeStr *str_copy(LakeStr *str);
LakeStr *str_from_c(char *s);
char *str_val(LakeStr *str);
LakeInt *str_len(LakeStr *str);
NILP str_set(LakeStr *str, char *s);
LakeInt *str_cmp(LakeStr *a, LakeStr *b);
LakeSym *str_eq(LakeStr *a, LakeStr *b);
LakeStr *str_to_str(LakeStr *str);

#endif