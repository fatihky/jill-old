#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "valindex.h"
#include "../utils/cont.h"

static struct jill_valindex_base **base_arr = NULL;
static int base_arr_len = 0;

int jill_valindex_global_init() {
  /*  register default types. */
  return 0;
}

int jill_valindex_last_internal_index() {
  return 0; /*  no internal valindex implementations yet. */
}

int jill_valindex_register (int type, struct jill_valindex_base *base) {
  assert (type >= 0);
  assert (base);
  /*  check if index exceeds array's length */
  if (type >= base_arr_len)
    return EINVAL;
  void *ptr = NULL;
  size_t nsize = (base_arr_len + 1) * sizeof(void *);
  ptr = realloc(base_arr, nsize);
  if (ptr == NULL)
    return ENOMEM;
  if (base_arr)
    memcpy (ptr, base_arr, nsize - sizeof(void *));

  /*  base_arr_len is not incremented yet. so we can use it as array's
      index. */
  ((struct jill_valindex_base **)ptr)[base_arr_len] = base;
  base_arr_len++;
  return 0;
}

struct jill_valindex *jill_valindex_create (int type, void *arg) {
  assert (type >= 0);
  assert (type < base_arr_len);
  return base_arr[type]->create (arg);
}

int jill_valindex_insert (struct jill_valindex *self,
    struct jill_value *value) {
  return self->base->insert (self, value);
}

int jill_valindex_set (struct jill_valindex *self, int index,
    struct jill_value *value) {
  return self->base->set (self, index, value);
}

int jill_valindex_get (struct jill_valindex *self, struct jill_value **value) {
  return self->base->get (self, value);
}

int jill_valindex_query (struct jill_valindex *self, void *query, void *result) {
  return self->base->query (self, query, result);
}
