#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "field.h"

void jill_field_init (struct jill_field *self) {
  self->name = NULL;
  jill_vallist_init (&self->vallist);
}

int jill_field_set_name (struct jill_field *self, char *name, size_t namelen) {
  char *ptr;
  ptr = malloc(namelen);
  if (ptr == NULL)
    return ENOMEM;
  memcpy (ptr, name, namelen);
  self->name = ptr;
  return 0;
}

void jill_field_term (struct jill_field *self) {
  if (self->name)
    free (self->name);
  jill_vallist_term (&self->vallist);
}

int jill_field_add_val (struct jill_field *self, void *lenp, void *val,
    size_t valsz) {
  int val_type = self->vallist.val_type;
  switch (val_type) {
    case JILL_VALLIST_FIXED_SIZE_VALS:
      return jill_vallist_add_fixed (&self->vallist, val);
    case JILL_VALLIST_LENGTH_PREFIXED_VALS:
      return jill_vallist_add_length_prefixed (&self->vallist, lenp, val,
        valsz);
    case JILL_VALLIST_BITMAP:
      return  jill_bitset_add (&self->vallist.bitset.bitset, *(int *)val);
    default: break;
  }
  return EINVAL;
}

int jill_field_get_bit (struct jill_field *self, int index) {
  assert (self->vallist.val_type == JILL_VALLIST_BITMAP);
  assert (index >= 0);
  return bitset_get (&self->vallist.bitset, (size_t) index);
}

int jill_field_get_fixed (struct jill_field *self, int index, void **valp) {
  assert (self->vallist.val_type == JILL_VALLIST_FIXED_SIZE_VALS);
  assert (index >= 0);
  size_t offset = (size_t)index * self->vallist.fixed_size_vals.val_size;
  /*  check if index is valid */
  if (index >= self->vallist.element_count)
    return EINVAL;
  *valp = (self->vallist.fixed_size_vals.elements + offset);
  return 0;
}
