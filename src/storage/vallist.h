#include <stdlib.h>

#ifndef JILL_VALLIST_H_INCLUDED
#define JILL_VALLIST_H_INCLUDED

#define JILL_VALLIST_FIXED_SIZE_VALS 1
#define JILL_VALLIST_LENGTH_PREFIXED_VALS 2
#define JILL_VALLIST_BITMAP 3 /*  basic bitmap */

struct jill_vallist {
  int val_type;
  int compressed;
  int element_count;
  int capacity;
  int grow; // grow bytes
  union {
    /**
     * Fixed size values.
     * suitable for: [u]int[8|16|32|64] integers types
     */
    struct {
      int val_size;
      char *elements;
    } fixed_size_vals;
    /**
     * Length prefixed values.
     * suitable for binary data
     */
     struct {
       /*  size of the length integer */
       int len_size;
       size_t value_buffer_size;
       char *lengths;
       char *value_buffer;
     } length_prefixed_vals;
     /**
      * Bitmaps
      */
    struct {
      struct jill_bitset bitset;
    } bitset;
     /**
      * Compressed data structures will be added...
      */
  };
};

void jill_vallist_init (struct jill_vallist *self);
void jill_vallist_term (struct jill_vallist *self);
void jill_vallist_set_grow (struct jill_vallist *self, int grow);
/*  set val_type and initialize structs. returns 0 on success. */
int jill_vallist_set_val_type (struct jill_vallist *self, int val_type);
void jill_vallist_set_fixed (struct jill_vallist *self, int val_size);
void jill_vallist_set_length_prefixed (struct jill_vallist *self,
  int len_size);
int jill_vallist_growby (struct jill_vallist *self, int grow);
int jill_vallist_add_fixed (struct jill_vallist *self, void *valp);
int jill_vallist_add_length_prefixed (struct jill_vallist *self, void *lenp,
  void *valp, size_t val_size);

#endif
