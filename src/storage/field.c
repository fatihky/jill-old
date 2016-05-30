#include <errno.h>
#include <stdlib.h>
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

int jill_field_add_val (struct jill_field *self, void *val, size_t *valsz) {
  if (self->vallist.val_type == JILL_VALLIST_FIXED_SIZE_VALS)
    return jill_vallist_add_fixed (&self->vallist, val);
  else if (self->vallist.val_type == JILL_VALLIST_LENGTH_PREFIXED_VALS)
    return jill_vallist_add_length_prefixed (&self->vallist, val, valsz);
}
