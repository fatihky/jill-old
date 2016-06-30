#include <string.h>
#include <errno.h>
#include "../core/common.h"
#include "../core/vallist.h"
#include "../utils/cont.h"
#include "common.h"
#include "distinct.h"

static struct jill_vallist *jill_vallist_distinct_create (void *arg);
static void jill_vallist_distinct_destroy (struct jill_vallist *self);
static int jill_vallist_distinct_setopt (struct jill_vallist *self, int option,
  const void *optval, size_t optvallen);
static int jill_vallist_distinct_getopt (struct jill_vallist *self, int option,
  void *optval, size_t *optvallen);
static int jill_vallist_distinct_run_custom_method (struct jill_vallist *self,
  int method, void *arg, void *result);
static int jill_vallist_distinct_insert (struct jill_vallist *self,
  struct jill_value *value);
static int jill_vallist_distinct_set (struct jill_vallist *self, int index,
  struct jill_value *value);
static int jill_vallist_distinct_get (struct jill_vallist *self, int index,
  struct jill_value **value);
static int jill_vallist_distinct_query (struct jill_vallist *self, void *query,
  void *result);

struct jill_vallist_base jill_vallist_distinct_base = {
  JILL_VALLIST_DISTINCT,
  jill_vallist_distinct_create,
  jill_vallist_distinct_destroy,
  jill_vallist_distinct_setopt,
  jill_vallist_distinct_getopt,
  jill_vallist_distinct_run_custom_method,
  jill_vallist_distinct_insert,
  jill_vallist_distinct_set,
  jill_vallist_distinct_get,
  jill_vallist_distinct_query
};

struct jill_vallist_distinct {
  struct jill_vallist vallist;
  int count;
  dict *valmap; /*  value > bitmap */
};

#define get_self(ptr) \
  jill_cont (ptr, struct jill_vallist_distinct, vallist)

/*  scan callbacks */
static void destroy_dictScanFn (void *privdata, const dictEntry *de) {
  (void) privdata;
  bitset_t *bs = (bitset_t *) de->v.val;
  bitset_free (bs);
}

static struct jill_vallist *jill_vallist_distinct_create (void *arg) {
  struct jill_vallist_distinct *vd = zcalloc(
    sizeof (struct jill_vallist_distinct));
  if (!vd)
    return NULL;
  vd->valmap = dictCreate (&dictTypeJillValToPtr, NULL);
  if (!vd->valmap) {
    zfree (vd);
    return NULL;
  }
  return &vd->vallist;
}

static void jill_vallist_distinct_destroy (struct jill_vallist *self) {
  struct jill_vallist_distinct *vd = get_self(self);
  dictRelease (vd->valmap);
  zfree (vd);
}

static int jill_vallist_distinct_setopt (struct jill_vallist *self, int option,
    const void *optval, size_t optvallen) {
  /*  not supported */
  return EINVAL;
}

static int jill_vallist_distinct_getopt (struct jill_vallist *self, int option,
    void *optval, size_t *optvallen) {
  /*  not supported */
  return EINVAL;
}

static int jill_vallist_distinct_run_custom_method (struct jill_vallist *self,
    int method, void *arg, void *result) {
  /*  not supported */
  return EINVAL;
}

static int _jill_vallist_distinct_set (
    struct jill_vallist_distinct *self, int index, struct jill_value *value) {
  int rc;
  bitset_t *bs;
  dictEntry *de;
  dict *vm = self->valmap;
  de = dictFind (vm, value);
  if (!de) {
    bs = bitset_create();
    if (!bs)
      return ENOMEM;
    rc = dictAdd (vm, value, bs);
    if (rc != 0)
      return ENOMEM;
    de = dictFind (vm, value);
  } else
    bs = (bitset_t *) de->v.val;
  bitset_set (bs, index);
  if (!bitset_get (bs, index))
    return ENOMEM;
  return 0;
}

static int jill_vallist_distinct_insert (struct jill_vallist *self,
    struct jill_value *value) {
  struct jill_vallist_distinct *vd = get_self(self);
  int rc;
  int index = vd->count;
  rc = _jill_vallist_distinct_set (vd, index, value);
  if (rc != 0)
    return rc;
  vd->count++;
  return 0;
}

static int jill_vallist_distinct_set (struct jill_vallist *self, int index,
    struct jill_value *value) {
  struct jill_vallist_distinct *vd = get_self(self);
  return _jill_vallist_distinct_set (vd, index, value);
}

static int jill_vallist_distinct_get (struct jill_vallist *self, int index,
    struct jill_value **value) {
  /*  not supported */
  return EINVAL;
}

static int jill_vallist_distinct_query (struct jill_vallist *self, void *query,
    void *result) {
  /*  not supported */
  return EINVAL;
}
