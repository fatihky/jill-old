#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "vallist.h"
#include "common.h"
#include "../utils/cont.h"
#include "../vallist/common.h"

static struct jill_vallist_base **base_arr = NULL;
static int base_arr_len = 0;

int jill_vallist_global_init() {
  /*  register default types. */
  int rc;
  rc = jill_vallist_register (JILL_VALLIST_FIXED, &jill_vallist_fixed_base);
  if (rc != 0)
    return rc;
  rc = jill_vallist_register (JILL_VALLIST_LENGTH_PREFIXED,
    &jill_vallist_length_prefixed_base);
  if (rc != 0)
    return rc;
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
  int diff;

  if (base_arr_len < expected_len) {
    size_t nsize = expected_len * sizeof(void *);
    ptr = zrealloc(base_arr, nsize);
    if (ptr == NULL)
      return ENOMEM;

    /*  mark difference area as NULL */
    diff = expected_len - base_arr_len - 1;
    for (int i = 0; i < diff; i++)
      ((void **)ptr)[i + base_arr_len] = NULL;

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
  if (type >= base_arr_len) {
    errno = EINVAL;
    return NULL;
  }
  if (!base_arr[type]) {
    errno = EINVAL;
    return NULL;
  }
  return base_arr[type]->create (arg);
}

void jill_vallist_destroy (struct jill_vallist *self) {
  return self->base->destroy (self);
}

int jill_vallist_insert (struct jill_vallist *self,
    struct jill_value *value) {
  return self->base->insert (self, value);
}

/*  set option */
int jill_vallist_setopt (struct jill_vallist *self, int option,
    const void *optval, size_t optvallen) {
  return self->base->setopt (self, option, optval, optvallen);
}

/*  get option */
int jill_vallist_getopt (struct jill_vallist *self, int option, void *optval,
    size_t *optvallen) {
  return self->base->getopt (self, option, optval, optvallen);
}

/*  call custom method */
int jill_vallist_run_custom_method (struct jill_vallist *self, int method,
    void *arg, void *result) {
  return self->base->run_custom_method (self, method, arg, result);
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
