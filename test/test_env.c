/**
  * test_env.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include "common.h"
#include "laketest.h"
#include "env.h"
#include "lake.h"

void setup(void);
static char *test_env_make(void);
static char *test_env_define(void);
static char *test_env_get(void);
static char *test_env_set(void);
static char *test_env_is_defined(void);

int tests_run;

static LakeCtx *lake;
static Env *toplevel;
static Env *firstlevel;
static LakeSym *s_answer;
static LakeVal *answer;
static LakeSym *s_undef;

int main(int argc, char const *argv[])
{
    setup();
    return !lt_run_tests("Environment", (test_fn[]){
        test_env_make,
        test_env_define,
        test_env_set,
        test_env_get,
        test_env_is_defined,
        NULL
    });
}

void setup(void)
{
    lake = lake_init();
    toplevel = lake->toplevel;
    firstlevel = env_make(toplevel);
    s_answer = sym_intern(lake, "answer");
    answer = VAL(int_from_c(42));
    s_undef = sym_intern(lake, "undefined");
}

/* Env *env_make(Env *parent) */
static char *test_env_make(void)
{
    lt_assert("toplevel is NULL", toplevel != NULL);
    lt_assert("toplevel->parent is not NULL", toplevel->parent == NULL);
    lt_assert("toplevel->bindings is NULL", toplevel->bindings != NULL);

    lt_assert("firstlevel is NULL", firstlevel != NULL);
    lt_assert("firstlevel->parent is not toplevel", firstlevel->parent == toplevel);

    return 0;
}

/* LakeVal *env_define(Env *env, LakeSym *key, LakeVal *val) */
static char *test_env_define(void)
{
    env_define(toplevel, s_answer, answer);
    lt_assert("env_define failed to define answer in toplevel",
              toplevel == env_is_defined(toplevel, s_answer));
    lt_assert("env_define set the value incorrectly",
              answer == env_get(toplevel, s_answer));
    return 0;
}

/* Env *env_is_defined(Env *env, LakeSym *key) */
static char *test_env_is_defined(void)
{
    /* unbound symbol */
    lt_assert("unbound symbol is defined",
              env_is_defined(toplevel, s_undef) == NULL);

    /* symbol bound in env itself */
    env_define(toplevel, s_answer, answer);
    lt_assert("failed to lookup symbol in defined env",
              toplevel == env_is_defined(toplevel, s_answer));

    /* symbol bound in parent */
    lt_assert("failed to lookup symbol in child env",
              toplevel == env_is_defined(firstlevel, s_answer));

    return 0;
}

/* LakeVal *env_set(Env *env, LakeSym *key, LakeVal *val) */
static char *test_env_set(void)
{
    /* unbound symbol */
    LakeVal *ret = env_set(toplevel, s_undef, answer);
    lt_assert("env_set returned non-NULL for an unbound symbol", ret == NULL);

    /* symbol bound in env itself */
    env_define(toplevel, s_answer, answer);
    LakeVal *zero = VAL(int_from_c(0));
    ret = env_set(toplevel, s_answer, zero);
    lt_assert("env_set failed to set bound symbol", ret != NULL);
    lt_assert("env_set failed to set bound symbol",
              zero == env_get(toplevel, s_answer));

    /* symbol bound in parent */
    ret = env_set(firstlevel, s_answer, answer);
    lt_assert("env_set failed to set symbol bound in parent", ret != NULL);
    lt_assert("env_set failed to set symbol bound in parent",
              answer == env_get(toplevel, s_answer));

    return 0;
}

/* LakeVal *env_get(Env *env, LakeSym *key) */
static char *test_env_get(void)
{
    /* unbound symbol */
    lt_assert("env_get returned non-NULL for an unbound symbol",
              NULL == env_get(toplevel, s_undef));

    /* symbol bound in env itself */
    env_define(toplevel, s_answer, answer);
    lt_assert("failed to get value", answer == env_get(toplevel, s_answer));

    /* symbol bound in parent */
    lt_assert("failed to get value defined in parent",
              answer == env_get(firstlevel, s_answer));

    return 0;
}
