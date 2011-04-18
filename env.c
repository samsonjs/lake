#include <stdio.h>
#include <stdlib.h>
#include "lake.h"
#include "env.h"
#include "hashtab.h"

#define ENV_TABLE_SIZE 64
#define SYM_TABLE_SIZE 1024

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
    Env *env;
    env = malloc(sizeof(Env));
    if (!env) oom();
    env->parent = parent;
    env->bindings = ht_init(ENV_TABLE_SIZE, NULL);
    env->symbols = ht_init(SYM_TABLE_SIZE, NULL);
    return env;
}

LakeVal *env_define(Env *env, char *key, LakeVal *val)
{
    size_t keylen = strlen(key);
    if (ht_get(env->bindings, key, keylen) != NULL)
        return NULL;
    
    return VAL_OR_NIL(ht_put(env->bindings, key, keylen, val, VAL_SIZE(val)));
}

LakeVal *env_set(Env *env, char *key, LakeVal *val)
{
    size_t keylen = strlen(key);
    if (ht_get(env->bindings, key, keylen) == NULL)
        return NULL;

    return VAL_OR_NIL(ht_put(env->bindings, key, keylen, val, VAL_SIZE(val)));
}

LakeVal *env_get(Env *env, char *key)
{
    return VAL_OR_NIL(ht_get(env->bindings, key, strlen(key)));
}

int env_is_bound(Env *env, char *key)
{
    return (ht_get(env->bindings, key, strlen(key)) != NULL);
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
        printf("unrecognized value, type %d, size %lu bytes", expr->type, expr->size);
        die("we don't eval that around here!");
    }
    
    return result;
}