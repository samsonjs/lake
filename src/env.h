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

#include <glib.h>

struct env {
    struct env *parent;
    GHashTable *bindings;
};
typedef struct env Env;

#include "lake.h"

Env *env_toplevel(void);

Env *env_make(Env *parent);
LakeVal *env_define(Env *env, LakeSym *key, LakeVal *val);
LakeVal *env_set(Env *env, LakeSym *key, LakeVal *val);
LakeVal *env_get(Env *env, LakeSym *key);
Env *env_is_defined(Env *env, LakeSym *key);

#endif
