#include <string.h>
#include <errno.h>
#include <assert.h>
#include "vallist.h"

void jill_vallist_init (struct jill_vallist *self) {
  memset (self, 0, sizeof (struct jill_vallist));
  self->val_type = -1;
}

void jill_vallist_term (struct jill_vallist *self) {
  assert (self);
  int has_el = self->element_count > 0;
  int has_capacity = self->capacity > 0;
  // fixed-size
  int fs = self->val_type == JILL_VALLIST_FIXED_SIZE_VALS;
  // length-prefixed
  int lp = self->val_type == JILL_VALLIST_LENGTH_PREFIXED_VALS;
  if (fs && has_el) {
    free (self->fixed_size_vals.elements);
  }
  else if (lp && (has_el || has_capacity)) {
    free (self->length_prefixed_vals.lengths);
    if (has_el)
      free (self->length_prefixed_vals.value_buffer);
  }
}

void jill_vallist_set_grow (struct jill_vallist *self, int grow) {
  assert (grow > 0);
  self->grow = grow;
}

int jill_vallist_set_val_type (struct jill_vallist *self, int val_type) {
  assert (self->val_type == -1);
  int rc;
  switch (val_type) {
    case JILL_VALLIST_BITMAP: {
      rc = jill_bitset_init (&self->bitset.bitset);
      if (rc != 0)
        return rc;
    } break;
    default: break;
  }
  self->val_type = val_type;
  return 0;
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

  self->capacity += grow;
  return 0;
}

int jill_vallist_lp_prealloc (struct jill_vallist *self, size_t bufsz) {
  assert (self->val_type == JILL_VALLIST_LENGTH_PREFIXED_VALS);
  size_t newsz = self->length_prefixed_vals.value_buffer_capacity + bufsz;
  void *ptr = realloc (self->length_prefixed_vals.value_buffer, newsz);
  if (ptr == NULL)
    return ENOMEM;
  self->length_prefixed_vals.value_buffer_capacity += bufsz;
  self->length_prefixed_vals.value_buffer = ptr;
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

  ptr = ((char *)self->fixed_size_vals.elements)
    + (size_t)((size_t)val_size * (size_t)self->element_count);
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
  size_t capacity = self->length_prefixed_vals.value_buffer_capacity;
  size_t needed = self->length_prefixed_vals.value_buffer_size + val_size;

  assert (self->val_type == JILL_VALLIST_LENGTH_PREFIXED_VALS);

  len_size = self->length_prefixed_vals.len_size;
  if (self->element_count == self->capacity) {
    rc = jill_vallist_growby (self, self->grow);
    if (rc < 0)
      return -rc;
  }

  ptr = self->length_prefixed_vals.value_buffer;

  if (capacity <= needed) {
    ptr = realloc(ptr, needed);
    if (ptr == NULL)
      return ENOMEM;
    /*  increase capacity */
    self->length_prefixed_vals.value_buffer_capacity += val_size;
  }

  tmp = ptr;
  tmp += self->length_prefixed_vals.value_buffer_size;
  memcpy(tmp, valp, val_size);
  self->length_prefixed_vals.value_buffer = ptr;
  self->length_prefixed_vals.value_buffer_size += val_size;

  ptr = (char *)self->length_prefixed_vals.lengths
    + (len_size * self->element_count);
  memcpy(ptr, lenp, len_size);
  self->element_count++;
  return 0;
}
