/** 
  * env.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "lake.h"
#include "env.h"
#include "symtable.h"

static Env *_top = NULL;

void env_set_toplevel(Env *env)
{
    _top = env;
}

Env *env_toplevel(void)
{
    if (!_top) {
        _top = env_make(NULL);
    }
    return _top;
}

Env *env_make(Env *parent)
{
    Env *env = g_malloc(sizeof(Env));
    env->parent = parent;
    env->bindings = symtable_make();
    return env;
}

Env *env_is_defined(Env *env, LakeSym *key)
{
    if (g_hash_table_lookup(env->bindings, key) != NULL) return env;
    return env->parent ? env_is_defined(env->parent, key) : NULL;
}

static void env_put(Env *env, LakeSym *key, LakeVal *val)
{
    g_hash_table_insert(env->bindings, key, val);
}

LakeVal *env_define(Env *env, LakeSym *key, LakeVal *val)
{
    if (env_is_defined(env, key) == env) {
        printf("warning: redefining %s\n", SYM_S(key));
    }
    env_put(env, key, val);
    return val;
}

LakeVal *env_set(Env *env, LakeSym *key, LakeVal *val)
{
    Env *definedEnv;
    if (!(definedEnv = env_is_defined(env, key))) {
        return NULL;
    }
    env_put(definedEnv, key, val);
    return val;
}

LakeVal *env_get(Env *env, LakeSym *key)
{
    LakeVal *val = g_hash_table_lookup(env->bindings, key);
    if (!val && env->parent) {
        val = env_get(env->parent, key);
    }
    if (!val) {
        ERR("undefined variable: %s", SYM_S(key));
        val = NULL;
    }
    return val;
}
