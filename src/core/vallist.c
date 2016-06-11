#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "vallist.h"
#include "../utils/cont.h"
#include "../vallist/common.h"

static struct jill_vallist_base **base_arr = NULL;
static int base_arr_len = 0;

int jill_vallist_global_init() {
  /*  register default types. */
  int rc;
  rc = jill_vallist_register (JILL_VALLIST_BITSET, &jill_vallist_bitset_base);
  if (rc != 0)
    return rc;
  return 0;
}

int jill_vallist_last_internal_index() {
  return JILL_VALLIST_MAX_ID;
}

int jill_vallist_register (int type, struct jill_vallist_base *base) {
  assert (type >= 0);
  assert (base);
  void *ptr = NULL;
  int expected_len = type + 1;

  if (base_arr_len < expected_len) {
    size_t nsize = expected_len * sizeof(void *);
    ptr = realloc(base_arr, nsize);
    if (ptr == NULL)
      return ENOMEM;
    /*  copy old data */
    if (base_arr)
      memcpy (ptr, base_arr, nsize - sizeof(void *));
    base_arr = ptr;
    base_arr_len = expected_len;
  }

  /*  base_arr_len is not incremented yet. so we can use it as array's
      index. */
  base_arr[type] = base;
  return 0;
}

struct jill_vallist *jill_vallist_create (int type, void *arg) {
  assert (type >= 0);
  assert (type < base_arr_len);
  return base_arr[type]->create (arg);
}

int jill_vallist_insert (struct jill_vallist *self,
    struct jill_value *value) {
  return self->base->insert (self, value);
}

int jill_vallist_set (struct jill_vallist *self, int index,
    struct jill_value *value) {
  return self->base->set (self, index, value);
}

int jill_vallist_get (struct jill_vallist *self, int index,
    struct jill_value **value) {
  return self->base->get (self, index, value);
}

int jill_vallist_query (struct jill_vallist *self, void *query, void *result) {
  return self->base->query (self, query, result);
}

void jill_vallist_destroy (struct jill_vallist *self) {
  return self->base->destroy (self);
}
