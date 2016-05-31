#include <assert.h>
#include <errno.h>
#include "jbitset.h"

int jill_bitset_init (struct jill_bitset *self) {
  self->index = 0;
  self->bitset = bitset_create();
  if (!self->bitset)
    return ENOMEM;
  return 0;
}

void jill_bitset_term (struct jill_bitset *self) {
  assert (self->index >= 0);
  bitset_free (self->bitset);
}

int jill_bitset_add (struct jill_bitset *self, int val) {
  assert (self->index >= 0);
  if (!val) {
    self->index++;
    return 0; // always return 0.
  }
  bitset_set (self->bitset, self->index);
  /*  we have to check if bit set or not. because bitset_set does not
      fail. */
  if (!bitset_get (self->bitset, self->index))
    return ENOMEM;
  self->index++;
  return 0;
}

bool jill_bitset_get (struct jill_bitset *self, int index) {
  return bitset_get (self->bitset, index);
}
