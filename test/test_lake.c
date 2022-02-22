/**
 * test_lake.c
 * Lake Scheme
 *
 * Copyright 2011 Sami Samhuri
 * MIT License
 *
 */

#include "bool.h"
#include "eval.h"
#include "int.h"
#include "lake.h"
#include "laketest.h"
#include "parse.h"
#include "str.h"
#include "sym.h"
#include <string.h>

void setup(void);
static char *test_lake_version(void);
static char *test_lake_init(void);
static char *test_lake_is(void);
static char *test_lake_equal(void);
static char *test_lake_repr(void);

static LakeCtx *lake;

int main(int argc, char const *argv[])
{
    setup();
    return !lt_run_tests("Lake", (test_fn[]){test_lake_version, test_lake_init,
                                             test_lake_is, test_lake_equal,
                                             test_lake_repr, NULL});
}

void setup(void) { lake = lake_init(); }

/* #define LAKE_VERSION "0.1" */
static char *test_lake_version(void)
{
    lt_assert("LAKE_VERSION is not set", NULL != LAKE_VERSION);
    return 0;
}

/* LakeCtx *lake_init(void) */
static char *test_lake_init(void)
{
    lt_assert("lake_init returned null", NULL != lake);
    lt_assert("toplevel is null", NULL != lake->toplevel);
    lt_assert("symbol table is null", NULL != lake->symbols);
    lt_assert("special form handlers map is null",
              NULL != lake->special_form_handlers);
    lt_assert("T is null", NULL != lake->T);
    lt_assert("F is null", NULL != lake->F);
    lt_assert("T is not a boolean", lake_is_type(TYPE_BOOL, lake->T));
    lt_assert("F is not a boolean", lake_is_type(TYPE_BOOL, lake->F));
    lt_assert("value of T is zero", lake_bool_val(lake->T));
    lt_assert("value of F is non-zero", !lake_bool_val(lake->F));
    return 0;
}

static bool _is(void *a, void *b) { return lake_is(VAL(a), VAL(b)); }

/* bool lake_is(LakeVal *a, LakeVal *b) */
static char *test_lake_is(void)
{
    LakeInt *i = int_from_c(42);

    // ints are compared by value
    lt_assert("ints with equal values are not the same",
              _is(i, int_from_c(42)));

    // nil is compared by value
    lt_assert("null values are not the same", _is(list_make(), list_make()));

    // everything else compared by reference
    LakeList *a = list_make();
    LakeList *b = list_make();
    list_append(a, VAL(i));
    list_append(b, VAL(i));
    lt_assert("distinct lists are the same", !_is(a, b));
    lt_assert("list is not itself", _is(a, a));

    return 0;
}

static bool _equal(void *a, void *b) { return lake_equal(VAL(a), VAL(b)); }

/* bool lake_equal(LakeVal *a, LakeVal *b) */
static char *test_lake_equal(void)
{
    LakeInt *i = int_from_c(42);
    LakeInt *j = int_from_c(42);
    LakeStr *arthur = lake_str_from_c("arthur");

    // values with different types are never equal
    lt_assert("values with different types are equal", !_equal(i, arthur));

    // symbols are compared by reference
    LakeSym *a = sym_intern(lake, "a");
    LakeSym *a2 = sym_intern(lake, "a");
    LakeSym *b = sym_intern(lake, "b");
    lt_assert("symbol is not equal to itself", _equal(a, a));
    lt_assert("symbol is not equal to itself", _equal(a, a2));
    lt_assert("different symbols are equal", !_equal(a, b));

    // booleans are compared by reference
    lt_assert("true is not equal to itself", _equal(lake->T, lake->T));
    lt_assert("true is equal to false", !_equal(lake->T, lake->F));

    // primitives are compared by reference
    LakePrimitive *null = PRIM(lt_eval(lake, "null?"));
    LakePrimitive *null2 = PRIM(lt_eval(lake, "null?"));
    LakePrimitive *pair = PRIM(lt_eval(lake, "pair?"));
    lt_assert("primitive is not equal to itself", _equal(null, null));
    lt_assert("primitive is not equal to itself", _equal(null, null2));
    lt_assert("different primitives are equal to each other",
              !_equal(null, pair));

    // functions are compared by reference
    LakeFn *inc = FN(lt_eval(lake, "(lambda (x) (+ x 1))"));
    LakeFn *inc1 = FN(lt_eval(lake, "(lambda (x) (+ x 1))"));
    lt_assert("functions is not equal to itself", _equal(inc, inc));
    lt_assert("distinct functions are equal to each other", !_equal(inc, inc1));

    // ints are compared by value
    lt_assert("ints with the same value are not equal", _equal(i, j));
    lt_assert("int is not equal to itself", _equal(i, i));

    // strings are compared by value
    LakeStr *arthur2 = lake_str_from_c("arthur");
    LakeStr *zaphod = lake_str_from_c("zaphod");
    lt_assert("string is not equal to itself", _equal(arthur, arthur));
    lt_assert("string is not equal to itself", _equal(arthur, arthur2));
    lt_assert("different strings are equal", !_equal(arthur, zaphod));

// lists are compared by value
#define S(s) VAL(lake_str_from_c(s))
    LakeList *fruits = list_make();
    list_append(fruits, S("mango"));
    list_append(fruits, S("pear"));
    list_append(fruits, S("pineapple"));

    LakeList *ninjas = list_make();
    list_append(ninjas, S("donatello"));
    list_append(ninjas, S("leonardo"));
    list_append(ninjas, S("michelangelo"));
    list_append(ninjas, S("raphael"));

    lt_assert("list is not equal to itself", _equal(fruits, fruits));
    lt_assert("different lists are equal", !_equal(fruits, ninjas));

    LakeList *fruits_copy = list_copy(fruits);
    lt_assert("copy of list is not equal to original",
              _equal(fruits, fruits_copy));
#undef S

    // dotted lists are compared by value
    LakeDottedList *destruction = dlist_make(fruits, VAL(ninjas));
    LakeDottedList *mayhem = dlist_make(ninjas, VAL(destruction));
    LakeDottedList *more_destruction = dlist_make(fruits, VAL(ninjas));
    lt_assert("dotted list is not equal to itself", _equal(mayhem, mayhem));
    lt_assert("different dotted lists are equal", !_equal(destruction, mayhem));
    lt_assert("identical dotted lists are not equal",
              _equal(destruction, more_destruction));

    // comments are compared by value
    LakeComment *comment = comment_from_c("a comment");
    LakeComment *different = comment_from_c("a different comment");
    LakeComment *same = comment_from_c("a comment");
    lt_assert("comment is not equal to itself", _equal(comment, comment));
    lt_assert("different comments are equal", !_equal(comment, different));
    lt_assert("identical comments are not equal", _equal(comment, same));

    // else FALSE
    lt_assert("LakeCtx was compared by lake_equal", !_equal(lake, lake));

    return 0;
}

/* char *lake_repr(LakeVal *val) */
static char *test_lake_repr(void)
{
    lt_assert("repr of NULL is not (null)",
              strncmp(lake_repr(NULL), "(null)", 6) == 0);
    lt_assert("repr of unknown value is not (unknown)",
              strncmp(lake_repr(NULL), "(null)", 6) == 0);

    // In every other case reading the the string returned by lake_repr should
    // result in a value equal to the original passed to lake_repr.
    LakeList *vals = list_make();
    list_append(vals, VAL(sym_intern(lake, "symbol")));
    list_append(vals, VAL(lake_str_from_c("string")));
    list_append(vals, VAL(lake_bool_from_int(lake, TRUE)));
    list_append(vals, VAL(int_from_c(42)));
    list_append(vals, VAL(vals));
    list_append(vals, VAL(dlist_make(vals, VAL(int_from_c(4919)))));
    list_append(vals, eval(lake, lake->toplevel, parse_expr(lake, "null?", 5)));
    list_append(vals, eval(lake, lake->toplevel,
                           parse_expr(lake, "(lambda xs xs)", 14)));
    list_append(vals, VAL(comment_from_c("this is a comment")));

    return 0;
}
