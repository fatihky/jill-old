#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "../core/vallist.h"
#include "../utils/cont.h"
#include "fixed.h"

#define JILL_VALLIST_GROW_DEFAULT 10

struct jill_vallist_fixed {
  struct jill_vallist vallist;
  int subtype;
  int custom_len;
  int element_count;
  int capacity;
  int grow;
  union {
    char *custom;
    int8_t *i8;
    uint8_t *u8;
    int16_t *i16;
    uint16_t *u16;
    int32_t *i32;
    uint32_t *u32;
    int64_t *i64;
    uint64_t *u64;
  } arr;
};

static struct jill_vallist *jill_vallist_fixed_create (void *arg);
static void jill_vallist_fixed_destroy (struct jill_vallist *self);
static int jill_vallist_fixed_insert (struct jill_vallist *self, struct jill_value *value);
static int jill_vallist_fixed_set (struct jill_vallist *self, int index,
  struct jill_value *value);
static int jill_vallist_fixed_get (struct jill_vallist *self, int index, struct jill_value **value);
static int jill_vallist_fixed_query (struct jill_vallist *self, void *query, void *result);

static int jill_vallist_fixed_one_el_size (struct jill_vallist_fixed *fx);

struct jill_vallist_base jill_vallist_fixed_base = {
  JILL_VALLIST_FIXED,
  jill_vallist_fixed_create,
  jill_vallist_fixed_destroy,
  jill_vallist_fixed_insert,
  jill_vallist_fixed_set,
  jill_vallist_fixed_get,
  jill_vallist_fixed_query
};

static struct jill_vallist *jill_vallist_fixed_create (void *arg_) {
  struct jill_vallist_fixed *fx;
  int *args = (int *)arg_; /*  int[2] */
  int one_el_sz;

  if (!args) {
    errno = EINVAL;
    return NULL;
  }

  fx = calloc (1, sizeof (struct jill_vallist_fixed));


  if (!fx) {
    errno = ENOMEM;
    return NULL;
  }

  fx->vallist.base = &jill_vallist_fixed_base;
  fx->grow = JILL_VALLIST_GROW_DEFAULT;
  fx->subtype = args[0];
  if (fx->subtype == JILL_VALLIST_FIXED_CUSTOM)
    fx->custom_len = args[1];

  one_el_sz = jill_vallist_fixed_one_el_size (fx);
  if (one_el_sz <= 0) {
    /*  invalid subtype */
    errno = EINVAL;
    free (fx);
    return NULL;
  }

  return &fx->vallist;
}

static int jill_vallist_fixed_one_el_size (struct jill_vallist_fixed *fx) {
  switch (fx->subtype) {
    case JILL_VALLIST_FIXED_CUSTOM:
      return fx->custom_len;
    case JILL_VALLIST_FIXED_I8:
    case JILL_VALLIST_FIXED_U8:
      return 1;
    case JILL_VALLIST_FIXED_I16:
    case JILL_VALLIST_FIXED_U16:
      return 2;
    case JILL_VALLIST_FIXED_I32:
    case JILL_VALLIST_FIXED_U32:
      return 4;
    case JILL_VALLIST_FIXED_I64:
    case JILL_VALLIST_FIXED_U64:
      return 8;
    default: break;
  }
  return -1; /*  unknown subtype */
}

static int jill_vallist_fixed_grow (struct jill_vallist_fixed *fx) {
  void *ptr = fx->arr.custom;
  size_t one_el_sz = jill_vallist_fixed_one_el_size (fx);
  size_t needed = one_el_sz * (fx->capacity + fx->grow);
  ptr = realloc (ptr, needed);
  if (!ptr)
    return ENOMEM;
  fx->arr.custom = ptr;
  fx->capacity += fx->grow;
  return 0;
}

static inline int _jill_vallist_fixed_set (struct jill_vallist_fixed *fx,
    int index, void *valp) {

  switch (fx->subtype) {
    case JILL_VALLIST_FIXED_I8:
      fx->arr.i8[index] = *(int8_t *)valp;
      break;
    case JILL_VALLIST_FIXED_U8:
      fx->arr.u8[index] = *(uint8_t *)valp;
      break;
    case JILL_VALLIST_FIXED_I16:
      fx->arr.i16[index] = *(int16_t *)valp;
      break;
    case JILL_VALLIST_FIXED_U16:
      fx->arr.u16[index] = *(uint16_t *)valp;
      break;
    case JILL_VALLIST_FIXED_I32:
      fx->arr.i32[index] = *(int32_t *)valp;
      break;
    case JILL_VALLIST_FIXED_U32:
      fx->arr.u32[index] = *(uint32_t *)valp;
      break;
    case JILL_VALLIST_FIXED_I64:
      fx->arr.i64[index] = *(int64_t *)valp;
      break;
    case JILL_VALLIST_FIXED_U64:
      fx->arr.u64[index] = *(uint64_t *)valp;
      break;
    case JILL_VALLIST_FIXED_CUSTOM: {
      int one_el_sz = jill_vallist_fixed_one_el_size (fx);
      char *ptr = fx->arr.custom + (index * one_el_sz);
      memcpy (ptr, valp, one_el_sz);
    }
    default:
      return EINVAL; /* unknown subtype */
  }

  return 0;
}

static int jill_vallist_fixed_insert (struct jill_vallist *self,
    struct jill_value *value) {
  struct jill_vallist_fixed *fx = jill_cont (self, struct jill_vallist_fixed,
    vallist);
  int rc;
  int must_grow = fx->element_count == fx->capacity;
  int index = fx->element_count;
  void *valp = value->valp;

  if (must_grow) {
    rc = jill_vallist_fixed_grow (fx);
    if (rc != 0)
      return rc;
  }

  rc = _jill_vallist_fixed_set (fx, index, valp);

  if (rc != 0)
    return rc;

  fx->element_count++;
  return 0;
}

static int jill_vallist_fixed_set (struct jill_vallist *self, int index,
    struct jill_value *value) {
  struct jill_vallist_fixed *fx = jill_cont (self, struct jill_vallist_fixed,
    vallist);
  /*  only updates allowed. no new values can be added with this method. */
  if (index > fx->element_count - 1)
    return EINVAL;
  return _jill_vallist_fixed_set (fx, index, value->valp);
}

static int _jill_vallist_fixed_get (struct jill_vallist_fixed *fx, int index,
    void *valp) {
  switch (fx->subtype) {
    case JILL_VALLIST_FIXED_I8:
      *(int8_t *)valp = fx->arr.i8[index];
      break;
    case JILL_VALLIST_FIXED_U8:
      *(uint8_t *)valp = fx->arr.u8[index];
      break;
    case JILL_VALLIST_FIXED_I16:
      *(int16_t *)valp = fx->arr.i16[index];
      break;
    case JILL_VALLIST_FIXED_U16:
      *(uint16_t *)valp = fx->arr.u16[index];
      break;
    case JILL_VALLIST_FIXED_I32:
      *(int32_t *)valp = fx->arr.i32[index];
      break;
    case JILL_VALLIST_FIXED_U32:
      *(uint32_t *)valp = fx->arr.u32[index];
      break;
    case JILL_VALLIST_FIXED_I64:
      *(int64_t *)valp = fx->arr.i64[index];
      break;
    case JILL_VALLIST_FIXED_U64:
      *(uint64_t *)valp = fx->arr.u64[index];
      break;
    default:
      return EINVAL;
  }
  return 0;
}

static int jill_vallist_fixed_get (struct jill_vallist *self, int index,
    struct jill_value **value) {
  struct jill_vallist_fixed *fx = jill_cont (self, struct jill_vallist_fixed,
    vallist);
  void *retp;
  if (!value || index < 0 || index > fx->element_count - 1)
    return EINVAL;
  retp = (*value)->valp;
  return _jill_vallist_fixed_get (fx, index, retp);
}

static int jill_vallist_fixed_query (struct jill_vallist *self, void *query,
    void *result) {
  /*  not supported */
  return EINVAL;
}

static void jill_vallist_fixed_destroy (struct jill_vallist *self) {
  struct jill_vallist_fixed *fx = jill_cont (self, struct jill_vallist_fixed,
    vallist);
  if (fx->arr.custom)
    free (fx->arr.custom);
  free (fx);
}
