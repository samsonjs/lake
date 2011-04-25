/** 
  * test_int.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <string.h>
#include "int.h"
#include "laketest.h"
#include "lake.h"

/* LakeInt *int_make(void) */
static char *test_int_make(void)
{
    LakeInt *i = int_make();
    lt_assert("int_make returned null", NULL != i);
    lt_assert("int_make did not return an integer", TYPE_INT == i->base.type);
    lt_assert("integer has incorrect size", sizeof(LakeInt) == i->base.size);
    lt_assert("integer was incorrectly initialized", 0 == INT_VAL(i));
    return 0;
}

/* LakeInt *int_from_c(int n) */
static char *test_int_from_c(void)
{
    LakeInt *i = int_from_c(42);
    lt_assert("integer was incorrectly initialized", 42 == INT_VAL(i));
    return 0;
}

/* LakeStr *int_repr(LakeInt *i) */
static char *test_int_repr(void)
{
    LakeInt *i = int_from_c(42);
    lt_assert("int_repr is wrong", strcmp(int_repr(i), "42") == 0);

    i = int_from_c(0);
    lt_assert("int_repr is wrong", strcmp(int_repr(i), "0") == 0);

    i = int_from_c(-42);
    lt_assert("int_repr is wrong", strcmp(int_repr(i), "-42") == 0);
    
    i = int_from_c(2147483647);
    lt_assert("int_repr is wrong", strcmp(int_repr(i), "2147483647") == 0);

    i = int_from_c(2147483648);
    lt_assert("int_repr is wrong", strcmp(int_repr(i), "-2147483648") == 0);
    
    return 0;
}

int main(int argc, char const *argv[])
{
    return !lt_run_tests("Integers", (test_fn[]){
        test_int_make,
        test_int_from_c,
        test_int_repr,
        NULL
    });
}