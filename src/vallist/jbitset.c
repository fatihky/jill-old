#include <assert.h>
#include <errno.h>
#include "jbitset.h"
#include "../core/vallist.h"
#include "../utils/cont.h"

/*  Uncompressed bitmap data structure */
struct jill_vallist_bitset {
  struct jill_vallist vallist;
  /*  last index to be marked. *auto incremented*
      can also used as element count property */
  int index;
  bitset_t *bitset;
};

static struct jill_vallist *jill_vallist_bitset_create (void *arg);
static void jill_vallist_bitset_destroy (struct jill_vallist *self);
static int jill_vallist_bitset_insert (struct jill_vallist *self,
  struct jill_value *value);
static int jill_vallist_bitset_set (struct jill_vallist *self, int index,
  struct jill_value *value);
static int jill_vallist_bitset_get (struct jill_vallist *self, int index,
  struct jill_value **value);
static int jill_vallist_bitset_query (struct jill_vallist *self, void *query,
  void *result);

struct jill_vallist_base jill_vallist_bitset_base = {
  JILL_VALLIST_BITSET,
  jill_vallist_bitset_create,
  jill_vallist_bitset_destroy,
  jill_vallist_bitset_insert,
  jill_vallist_bitset_set,
  jill_vallist_bitset_get,
  jill_vallist_bitset_query
};

static struct jill_vallist *jill_vallist_bitset_create (void *arg) {
  (void) arg;
  struct jill_vallist_bitset *bs = NULL;
  bs = malloc (sizeof (struct jill_vallist_bitset));
  if (!bs)
    goto enomem;

  bs->index = 0;
  bs->vallist.base = &jill_vallist_bitset_base;
  bs->bitset = bitset_create();
  if (!bs->bitset)
    goto enomem;

  return &bs->vallist;

enomem:
  if (bs)
    free (bs);
  errno = ENOMEM;
  return NULL;
}

static int jill_vallist_bitset_insert (struct jill_vallist *self,
    struct jill_value *value) {

  struct jill_vallist_bitset *bs = jill_cont (self, struct jill_vallist_bitset,
    vallist);

  if (!value) {
    bs->index++;
    return 0;
  }

  bitset_set (bs->bitset, bs->index);
  /*  we have to check if bit set or not. because bitset_set does not
      fail. */
  if (!bitset_get (bs->bitset, bs->index))
    return ENOMEM;
  bs->index++;
  return 0;
}

static int jill_vallist_bitset_set (struct jill_vallist *self, int index,
    struct jill_value *value) {
  struct jill_vallist_bitset *bs = jill_cont (self, struct jill_vallist_bitset,
    vallist);
  bitset_set (bs->bitset, index);
  if (!bitset_get (bs->bitset, index))
    return ENOMEM;
  return 0;
}

static int jill_vallist_bitset_get (struct jill_vallist *self, int index,
    struct jill_value **value) {
  struct jill_vallist_bitset *bs = jill_cont (self, struct jill_vallist_bitset,
    vallist);
  int *retp;

  if (!value || index < 0)
    return EINVAL;

  retp = (int *)((*value)->valp);

  if (index > bs->index - 1) {
    *retp = 0;
    return 0;
  }

  if (bitset_get (bs->bitset, index))
    *retp = 1;
  else
    *retp = 0;

  return 0;
}

/*  not supported yet */
static int jill_vallist_bitset_query (struct jill_vallist *self, void *query,
    void *result) {
  return EINVAL;
}

static void jill_vallist_bitset_destroy (struct jill_vallist *self) {
  struct jill_vallist_bitset *bs = jill_cont (self, struct jill_vallist_bitset,
    vallist);
  bitset_free (bs->bitset);
  free (bs);
}
