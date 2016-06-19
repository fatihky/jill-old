#include <stdlib.h>
#include "common.h"
#include "skiplist.h"

void jill_skiplist_init (struct jill_skiplist *self) {
  self->element_count = 0;
  self->elements = NULL;
}

void jill_skiplist_term (struct jill_skiplist *self) {
  if (self->element_count > 0)
    zfree (self->elements);
}