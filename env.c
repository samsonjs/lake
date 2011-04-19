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
#include <string.h>
#include "lake.h"
#include "env.h"

static Env *_shared = NULL;

void set_shared_env(Env *env)
{
    _shared = env;
}

Env *shared_env(void)
{
    if (!_shared) {
        _shared = env_make(NULL);
    }
    return _shared;
}

Env *env_make(Env *parent)
{
    Env *env = g_malloc(sizeof(Env));
    env->parent = parent;
    env->bindings = g_hash_table_new(g_str_hash, g_str_equal);
    env->symbols = g_hash_table_new(g_str_hash, g_str_equal);
    return env;
}

LakeVal *env_define(Env *env, char *key, LakeVal *val)
{
    if (g_hash_table_lookup(env->bindings, key) != NULL)
        return NULL;
    
	val = VAL_OR_NIL(val);
	g_hash_table_insert(env->bindings, key, val);
    return val;
}

LakeVal *env_set(Env *env, char *key, LakeVal *val)
{
    if (g_hash_table_lookup(env->bindings, key) == NULL)
        return NULL;

	val = VAL_OR_NIL(val);
	g_hash_table_insert(env->bindings, key, val);
    return val;
}

LakeVal *env_get(Env *env, char *key)
{
    return VAL_OR_NIL(g_hash_table_lookup(env->bindings, key));
}

int env_is_bound(Env *env, char *key)
{
    return g_hash_table_lookup(env->bindings, key) != NULL;
}

/*
LakeVal *env_apply(Env *env, LakeFn *fn, LakeList *args)
{
    return NIL;
}
*/

LakeVal *env_eval(Env *env, LakeVal *expr)
{
    LakeVal *result;
    LakeList *list;
    
    switch (expr->type) {

    /* self evaluating types */
    case TYPE_NIL:
    case TYPE_SYM:
    case TYPE_BOOL:
    case TYPE_INT:
    case TYPE_STR:
        result = expr;
        break;

    case TYPE_LIST:
        list = LIST(expr);
        if (LIST_N(list) == 0) {
            result = NIL;
        } else {
            /*
            LakeFn *fn = list_shift(list);
            result = env_apply(env, fn, list);
            */
            printf("%s:%d TODO apply functions\n", __FILE__, __LINE__);
            result = expr;
        }
        break;
        
    default:
        printf("unrecognized value, type %d, size %Zu bytes", expr->type, expr->size);
        die("we don't eval that around here!");
    }
    
    return result;
}