#include "bitset.h"

#ifndef JILL_BITSET_H_INCLUDED
#define JILL_BITSET_H_INCLUDED

/**
 * Uncompressed bitmap data structure
 */

struct jill_bitset {
  /*  last index to be marked. *auto incremented*
      can also used as element count property */
  int index;
  bitset_t *bitset;
};

int jill_bitset_init (struct jill_bitset *self);
void jill_bitset_term (struct jill_bitset *self);
/*  value of new bit. */
int jill_bitset_add (struct jill_bitset *self, int val);
bool jill_bitset_get (struct jill_bitset *self, int index);

#endif
