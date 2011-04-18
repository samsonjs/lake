/** 
  * env.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  */

#ifndef _LAKE_ENV_H
#define _LAKE_ENV_H 1

#include "hashtab.h"

struct env {
    struct env *parent;
    hashtab_t *bindings;
    hashtab_t *symbols;
};
typedef struct env Env;

#include "lake.h"

void set_shared_env(Env *env);
Env *shared_env(void);

Env *env_make(Env *parent);
LakeVal *env_define(Env *env, char *key, LakeVal *val);
LakeVal *env_set(Env *env, char *key, LakeVal *val);
LakeVal *env_get(Env *env, char *key);
int env_is_bound(Env *env, char *key);
LakeVal *env_eval(Env *env, LakeVal *expr);

#endif
