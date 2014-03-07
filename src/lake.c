/** 
  * lake.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  * A quick and dirty scheme written in C, for fun and to use while
  * reading The Little Schemer.
  *
  */

#include "bool.h"
#include "comment.h"
#include "common.h"
#include "hash.h"
#include "env.h"
#include "eval.h"
#include "lake.h"
#include "list.h"
#include "primitive.h"
#include "str.h"

int lake_val_size(void *x)
{
  return VAL(x)->size;
}

int lake_is_type(LakeType t, void *x)
{
  return VAL(x)->type == t;
}

char *lake_repr(void *expr)
{
  if (expr == NULL) return strdup("(null)");

  char *s = NULL;

  LakeVal *e = VAL(expr);
  switch (e->type) {

    case TYPE_SYM:
      s = sym_repr(SYM(e));
      break;

    case TYPE_BOOL:
      s = lake_bool_repr(BOOL(e));
      break;

    case TYPE_INT:
      s = int_repr(INT(e));
      break;

    case TYPE_STR: {
        size_t n = strlen(STR_S(STR(e))) + 2;
        s = malloc(n);
          /* TODO: quote the string */
        snprintf(s, n, "\"%s\"", STR_S(STR(e)));
        break;
      }

    case TYPE_LIST:
      s = list_repr(LIST(e));
      break;

    case TYPE_DLIST:
      s = dlist_repr(DLIST(e));
      break;

    case TYPE_PRIM:
      s = prim_repr(PRIM(e));
      break;

    case TYPE_FN:
      s = fn_repr(FN(e));
      break;

    case TYPE_COMM:
      s = comment_repr(COMM(e));
      break;

    default:
      // If it wasn't a LakeVal we already crashed at the beginning of the switch,
      // so go ahead and print out the size too.
      fprintf(stderr, "error: unrecognized value, type %d, size %zu bytes",
      e->type, e->size);
      s = strdup("(unknown)");
  }

  return s;
}

bool lake_is_nil(LakeVal *x)
{
  return lake_is_type(TYPE_LIST, x) && LIST_N(LIST(x)) == 0;
}

bool lake_is(LakeVal *a, LakeVal *b)
{
  if (lake_is_type(TYPE_INT, a) && lake_is_type(TYPE_INT, b)) {
    return INT_VAL(INT(a)) == INT_VAL(INT(b));
  }
  if (lake_is_nil(a) && lake_is_nil(b)) return TRUE;
  return a == b;
}

static char *type_name(LakeVal *expr)
{
  static char *type_names[9] = { "nil", "symbol", "boolean", "integer", "string", "list",
    "dotted-list", "primitive", "function"
  };

  LakeType t = expr->type;
  return t >= 0 && t <= 8 ? type_names[t] : "(not a LakeVal)";
}

bool lake_equal(LakeVal *a, LakeVal *b)
{
  if (a->type != b->type) return FALSE;
  switch (a->type) {

    /* singletons can be compared directly */
    case TYPE_SYM:
    case TYPE_BOOL:
    case TYPE_PRIM:
    case TYPE_FN:
      return a == b;

    case TYPE_INT:
      return INT_VAL(INT(a)) == INT_VAL(INT(b));

    case TYPE_STR:
      return lake_str_equal(STR(a), STR(b));

    case TYPE_LIST:
      return list_equal(LIST(a), LIST(b));

    case TYPE_DLIST:
      return dlist_equal(DLIST(a), DLIST(b));

    case TYPE_COMM:
      return comment_equal(COMM(a), COMM(b));

    default:
      ERR("unknown type %d (%s)", a->type, type_name(a));
      return FALSE;
  }
}

static LakeBool *bool_make(bool val)
{
  LakeBool *b = malloc(sizeof(LakeBool));
  VAL(b)->type = TYPE_BOOL;
  VAL(b)->size = sizeof(LakeBool);
  b->val = val;
  return b;
}

LakeCtx *lake_init(void)
{
  LakeCtx *ctx = malloc(sizeof(LakeCtx));
  ctx->toplevel = env_make(NULL);
  ctx->symbols = lake_hash_make();
  ctx->special_form_handlers = lake_hash_make();
  ctx->T = bool_make(TRUE);
  ctx->F = bool_make(FALSE);
  bind_primitives(ctx);
  init_special_form_handlers(ctx);
  return ctx;
}
