#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "field.h"

int jill_field_init (struct jill_field *self) {
  self->name = NULL;
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
}
