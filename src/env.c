/** 
  * env.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "hash.h"
#include "lake.h"
#include "env.h"

Env *env_make(Env *parent)
{
  Env *env = malloc(sizeof(Env));
  env->parent = parent;
  env->bindings = lk_hash_make();
  return env;
}

Env *env_is_defined(Env *env, LakeSym *key)
{
  if (lk_hash_get(env->bindings, key->s) != NULL) return env;
  return env->parent ? env_is_defined(env->parent, key) : NULL;
}

static void env_put(Env *env, LakeSym *key, LakeVal *val)
{
  lk_hash_put(env->bindings, key->s, val);
}

LakeVal *env_define(Env *env, LakeSym *key, LakeVal *val)
{
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
  LakeVal *val = lk_hash_get(env->bindings, key->s);
  if (!val && env->parent) {
    val = env_get(env->parent, key);
  }
  return val;
}
