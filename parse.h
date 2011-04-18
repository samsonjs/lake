#ifndef _PARSE_H
#define _PARSE_H 1

#include <stdlib.h>
#include "lake.h"

LakeVal *parse_expr(char *s, size_t n);

#endif