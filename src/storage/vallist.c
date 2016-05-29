#include <string.h>
#include <errno.h>
#include <assert.h>
#include "vallist.h"

void jill_vallist_init (struct jill_vallist *self) {
  memset (self, 0, sizeof (struct jill_vallist));
  self->val_type = -1;
}

void jill_vallist_term (struct jill_vallist *self) {}

void jill_vallist_set_grow (struct jill_vallist *self, int grow) {
  assert (grow > 0);
  self->grow = grow;
}

void jill_vallist_set_fixed (struct jill_vallist *self, int val_size) {
  assert (self->val_type == -1);
  self->val_type = JILL_VALLIST_FIXED_SIZE_VALS;
  self->fixed_size_vals.val_size = val_size;
}

void jill_vallist_set_length_prefixed (struct jill_vallist *self,
    int len_size) {
  assert (self->val_type == -1);
  self->val_type = JILL_VALLIST_LENGTH_PREFIXED_VALS;
  self->length_prefixed_vals.len_size = len_size;
}

int jill_vallist_growby (struct jill_vallist *self, int grow) {
  void *ptr;
  size_t size;
  size_t one_el_size;
  assert (grow > 0);
  assert (self->val_type == JILL_VALLIST_FIXED_SIZE_VALS ||
    self->val_type == JILL_VALLIST_LENGTH_PREFIXED_VALS);
  if (self->val_type == JILL_VALLIST_FIXED_SIZE_VALS) {
    size = self->fixed_size_vals.val_size;
    ptr = self->fixed_size_vals.elements;
  }
  else {
    size = self->length_prefixed_vals.len_size;
    ptr = self->length_prefixed_vals.lengths;
  }

  size = size * (self->capacity + grow);

  ptr = realloc(ptr, size);
  if (ptr == NULL)
    return -ENOMEM;

  if (self->val_type == JILL_VALLIST_FIXED_SIZE_VALS)
    self->fixed_size_vals.elements = ptr;
  else
    self->length_prefixed_vals.lengths = ptr;

  return 0;
}

int jill_vallist_add_fixed (struct jill_vallist *self, void *valp) {
  int rc;
  int val_size;
  void *ptr;

  assert (self->val_type == JILL_VALLIST_FIXED_SIZE_VALS);

  val_size = self->fixed_size_vals.val_size;
  if (self->element_count == self->capacity) {
    rc = jill_vallist_growby (self, self->grow);
    if (rc < 0)
      return -rc;
  }

  ptr = (char *)self->fixed_size_vals.elements
    + (val_size * self->element_count);
  memcpy(ptr, valp, val_size);
  self->element_count++;
  return 0;
}

int jill_vallist_add_length_prefixed (struct jill_vallist *self, void *lenp,
    void *valp, size_t val_size) {
  int rc;
  int len_size;
  void *ptr;
  void *tmp;

  assert (self->val_type == JILL_VALLIST_LENGTH_PREFIXED_VALS);

  len_size = self->length_prefixed_vals.len_size;
  if (self->element_count == self->capacity) {
    rc = jill_vallist_growby (self, self->grow);
    if (rc < 0)
      return -rc;
  }

  ptr = self->length_prefixed_vals.value_buffer;
  ptr = realloc(ptr, self->length_prefixed_vals.value_buffer_size + val_size);
  if (ptr == NULL)
    return ENOMEM;

  tmp = ptr;
  tmp += self->length_prefixed_vals.value_buffer_size;
  memcpy(tmp, valp, val_size);
  ptr -= self->length_prefixed_vals.value_buffer_size;
  self->length_prefixed_vals.value_buffer = ptr;
  self->length_prefixed_vals.value_buffer_size += val_size;

  ptr = (char *)self->length_prefixed_vals.lengths
    + (len_size * self->element_count);
  memcpy(ptr, lenp, len_size);
  self->element_count++;
  return 0;
}
