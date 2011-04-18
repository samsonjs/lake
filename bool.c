#include "bool.h"
#include "lake.h"
#include "sym.h"

LakeSym *bool_from_int(int n)
{
    return n ? sym_intern("#t") : sym_intern("#f");
}

int is_true(LakeVal *val)
{
    return (val->type == TYPE_SYM && sym_eq(sym_intern("#t"), SYM(val)));
}

int is_false(LakeVal *val)
{
    return (val->type == TYPE_SYM && sym_eq(sym_intern("#f"), SYM(val)));
}

int is_truthy(LakeVal *val)
{
    return is_true(val); /* TODO */
}

int is_falsy(LakeVal *val)
{
    return is_false(val); /* TODO */
}
