#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "../core/vallist.h"
#include "../utils/cont.h"
#include "len_prefixed.h"

#define JILL_VALLIST_LENGTH_PREFIXED_DEFAULT_GROW 10

static struct jill_vallist *jill_vallist_length_prefixed_create (void *arg);
static int jill_vallist_length_prefixed_insert (struct jill_vallist *self,
  struct jill_value *value);
static int jill_vallist_length_prefixed_set (struct jill_vallist *self,
  int index, struct jill_value *value);
static int jill_vallist_length_prefixed_get (struct jill_vallist *self,
  int index, struct jill_value **value);
static int jill_vallist_length_prefixed_query (struct jill_vallist *self,
  void *query, void *result);
static void jill_vallist_length_prefixed_destroy (struct jill_vallist *self);

struct jill_vallist_base jill_vallist_length_prefixed_base = {
  JILL_VALLIST_LENGTH_PREFIXED,
  jill_vallist_length_prefixed_create,
  jill_vallist_length_prefixed_insert,
  jill_vallist_length_prefixed_set,
  jill_vallist_length_prefixed_get,
  jill_vallist_length_prefixed_query,
  jill_vallist_length_prefixed_destroy
};

struct jill_vallist_length_prefixed {
  struct jill_vallist vallist;
  int subtype;
  int lensz;
  int count;
  int grow;
  size_t lenbuf_capacity;
  size_t lenbuf_size;
  size_t valbuf_capacity;
  size_t valbuf_size;
  char *lenbuf;
  char *valbuf;
};

static struct jill_vallist *jill_vallist_length_prefixed_create (void *arg) {
  struct jill_vallist_length_prefixed *lp;
  int subtype;

  if (!arg)
    goto einval;

  subtype = *(int *)arg;
  if (subtype < 0 || subtype > JILL_VALLIST_LENGTH_PREFIXED_U32)
    goto einval;

  lp = calloc (1, sizeof (struct jill_vallist_length_prefixed));
  if (!lp) {
    errno = ENOMEM;
    return NULL;
  }

  lp->subtype = subtype;
  lp->grow = JILL_VALLIST_LENGTH_PREFIXED_DEFAULT_GROW;

  return &lp->vallist;

einval:
  errno = EINVAL;
  return NULL;
}

static inline int jill_vallist_length_prefixed_lensz (
    struct jill_vallist_length_prefixed *self) {
  switch (self->subtype) {
    case JILL_VALLIST_LENGTH_PREFIXED_I8:
    case JILL_VALLIST_LENGTH_PREFIXED_U8: return 1;
    case JILL_VALLIST_LENGTH_PREFIXED_I16:
    case JILL_VALLIST_LENGTH_PREFIXED_U16: return 2;
    case JILL_VALLIST_LENGTH_PREFIXED_I32:
    case JILL_VALLIST_LENGTH_PREFIXED_U32: return 4;
    default: break;
  }
  /*  unknown subtype */
  return -1;
}

static int jill_vallist_length_prefixed_grow_lenbuf (
    struct jill_vallist_length_prefixed *self, int growto) {
  int lensz = jill_vallist_length_prefixed_lensz (self);
  size_t needed = (self->count > growto ? self->count : growto) * lensz;
  if (self->lenbuf_capacity >= needed)
    return 0;

  void *ptr = self->lenbuf;
  ptr = realloc (ptr, needed);

  if (!ptr)
    return ENOMEM;

  self->lenbuf_capacity = needed;
  self->lenbuf = ptr;

  return 0;
}

static inline size_t jill_vallist_length_prefixed_len (
    struct jill_vallist_length_prefixed *self, void *lenp) {
  switch (self->subtype) {
    case JILL_VALLIST_LENGTH_PREFIXED_I8:
      return (size_t) *(int8_t *) lenp;
    case JILL_VALLIST_LENGTH_PREFIXED_U8:
      return (size_t) *(uint8_t *) lenp;
    case JILL_VALLIST_LENGTH_PREFIXED_I16:
      return (size_t) *(int16_t *) lenp;
    case JILL_VALLIST_LENGTH_PREFIXED_U16:
      return (size_t) *(uint16_t *) lenp;
    case JILL_VALLIST_LENGTH_PREFIXED_I32:
      return (size_t) *(int32_t *) lenp;
    case JILL_VALLIST_LENGTH_PREFIXED_U32:
      return (size_t) *(uint32_t *) lenp;
    default: break;
  }
  return 0;
}

static inline void jill_vallist_length_prefixed_encode_length (
    struct jill_vallist_length_prefixed *lp, int index, void *lenp) {
  switch (lp->subtype) {
    case JILL_VALLIST_LENGTH_PREFIXED_I8:
      ((int8_t *)lp->lenbuf)[index] = *(int8_t *) lenp;
    case JILL_VALLIST_LENGTH_PREFIXED_U8:
      ((uint8_t *)lp->lenbuf)[index] = *(uint8_t *) lenp;
    case JILL_VALLIST_LENGTH_PREFIXED_I16:
      ((int16_t *)lp->lenbuf)[index] = *(int16_t *) lenp;
    case JILL_VALLIST_LENGTH_PREFIXED_U16:
      ((uint16_t *)lp->lenbuf)[index] = *(uint16_t *) lenp;
    case JILL_VALLIST_LENGTH_PREFIXED_I32:
      ((int32_t *)lp->lenbuf)[index] = *(int32_t *) lenp;
    case JILL_VALLIST_LENGTH_PREFIXED_U32:
      ((uint32_t *)lp->lenbuf)[index] = *(uint32_t *) lenp;
    default: break;
  }
}

static int jill_vallist_length_prefixed_insert (struct jill_vallist *self,
    struct jill_value *value) {
  struct jill_vallist_length_prefixed *lp = jill_cont (self,
    struct jill_vallist_length_prefixed, vallist);
  size_t needed;
  size_t vallen = jill_vallist_length_prefixed_len (lp, value->lenp);
  int rc;
  int index = lp->count;

  /*  grow lenbuf if necessary */
  if (lp->lenbuf_capacity == lp->lenbuf_size) {
    rc = jill_vallist_length_prefixed_grow_lenbuf (lp, lp->count + lp->grow);
    if (rc != 0)
      return rc;
  }

  /*  grow valbuf if necessary */
  needed = lp->valbuf_size + vallen;
  if (needed > lp->valbuf_capacity) {
    void *ptr = lp->valbuf;
    ptr = realloc (ptr, needed);
    if (!ptr)
      return ENOMEM;
    lp->valbuf = ptr;
    lp->valbuf_capacity = needed;
  }

  /*  encode length */
  jill_vallist_length_prefixed_encode_length (lp, index, value->lenp);
  lp->lenbuf_size += lp->lensz;

  /*  encode the value */
  memcpy (lp->valbuf + lp->valbuf_size, value->valp, vallen);
  lp->valbuf_size += vallen;
  return 0;
}

static int jill_vallist_length_prefixed_set (struct jill_vallist *self,
    int index, struct jill_value *value) {
  /*  not supported yet */
  return EINVAL;
}

static int jill_vallist_length_prefixed_get (struct jill_vallist *self,
    int index, struct jill_value **value) {
  /*  not supported yet */
  return EINVAL;
}

static int jill_vallist_length_prefixed_query (struct jill_vallist *self,
    void *query, void *result) {
  /*  not supported yet */
  return EINVAL;
}

static void jill_vallist_length_prefixed_destroy (struct jill_vallist *self) {
  struct jill_vallist_length_prefixed *lp = jill_cont (self,
    struct jill_vallist_length_prefixed, vallist);
  if (lp->lenbuf)
    free (lp->lenbuf);
  if (lp->valbuf)
    free (lp->valbuf);
  free (lp);
}