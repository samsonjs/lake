/** 
  * test_env.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include "minunit.h"
#include "env.h"
#include "lake.h"

int tests_run;

static LakeCtx *lake;
static Env *toplevel;
static Env *firstlevel;
static LakeSym *s_answer;
static LakeVal *answer;
static LakeSym *s_undef;

/* Env *env_make(Env *parent) */
static char *test_env_make(void)
{
    mu_assert("toplevel is NULL", toplevel != NULL);
    mu_assert("toplevel->parent is not NULL", toplevel->parent == NULL);
    mu_assert("toplevel->bindings is NULL", toplevel->bindings != NULL);

    mu_assert("firstlevel is NULL", firstlevel != NULL);
    mu_assert("firstlevel->parent is not toplevel", firstlevel->parent == toplevel);
    
    return 0;
}

/* LakeVal *env_define(Env *env, LakeSym *key, LakeVal *val) */
static char *test_env_define(void)
{
    env_define(toplevel, s_answer, answer);
    mu_assert("env_define failed to define answer in toplevel",
              toplevel == env_is_defined(toplevel, s_answer));
    mu_assert("env_define set the value incorrectly",
              answer == env_get(toplevel, s_answer));
    return 0;
}

/* Env *env_is_defined(Env *env, LakeSym *key) */
static char *test_env_is_defined(void)
{
    /* unbound symbol */
    mu_assert("unbound symbol is defined",
              env_is_defined(toplevel, s_undef) == NULL);
    
    /* symbol bound in env itself */
    env_define(toplevel, s_answer, answer);
    mu_assert("failed to lookup symbol in defined env",
              toplevel == env_is_defined(toplevel, s_answer));
    
    /* symbol bound in parent */
    mu_assert("failed to lookup symbol in child env",
              toplevel == env_is_defined(firstlevel, s_answer));
    
    return 0;
}

/* LakeVal *env_set(Env *env, LakeSym *key, LakeVal *val) */
static char *test_env_set(void)
{
    /* unbound symbol */
    LakeVal *ret = env_set(toplevel, s_undef, answer);
    mu_assert("env_set returned non-NULL for an unbound symbol", ret == NULL);
    
    /* symbol bound in env itself */
    env_define(toplevel, s_answer, answer);
    LakeVal *zero = VAL(int_from_c(0));
    ret = env_set(toplevel, s_answer, zero);
    mu_assert("env_set failed to set bound symbol", ret != NULL);
    mu_assert("env_set failed to set bound symbol",
              zero == env_get(toplevel, s_answer));
    
    /* symbol bound in parent */
    ret = env_set(firstlevel, s_answer, answer);
    mu_assert("env_set failed to set symbol bound in parent", ret != NULL);
    mu_assert("env_set failed to set symbol bound in parent",
              answer == env_get(toplevel, s_answer));
    
    return 0;
}

/* LakeVal *env_get(Env *env, LakeSym *key) */
static char *test_env_get(void)
{
    /* unbound symbol */
    mu_assert("env_get returned non-NULL for an unbound symbol",
              NULL == env_get(toplevel, s_undef));

    /* symbol bound in env itself */
    env_define(toplevel, s_answer, answer);
    mu_assert("failed to get value", answer == env_get(toplevel, s_answer));

    /* symbol bound in parent */
    mu_assert("failed to get value defined in parent",
              answer == env_get(firstlevel, s_answer));
    
    return 0;
}

static void setup(void)
{
    lake = lake_init();
    toplevel = lake->toplevel;
    firstlevel = env_make(toplevel);
    s_answer = sym_intern(lake, "answer");
    answer = VAL(int_from_c(42));
    s_undef = sym_intern(lake, "undefined");
}

static char *all_tests() {
    setup();
    mu_run_test(test_env_make);
    mu_run_test(test_env_define);
    mu_run_test(test_env_set);
    mu_run_test(test_env_get);
    mu_run_test(test_env_is_defined);
    return 0;
}

int main(int argc, char const *argv[]) {
    char *result = all_tests();
    int pass = result == 0;
    printf("-- Environment --\n");
    if (pass) {
        printf("PASS: %d test%s run\n", tests_run, tests_run == 1 ? "" : "s");
    }
    else {
        printf("%s\n", result);
        printf("FAIL: %d test%s run\n", tests_run, tests_run == 1 ? "" : "s");
    }

    return !pass;
}

