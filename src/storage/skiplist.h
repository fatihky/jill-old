#ifndef JILL_SKIPLIST_H_INCLUDED
#define JILL_SKIPLIST_H_INCLUDED

struct jill_skiplist {
  uint8_t element_count;
  // this is the array of the int[2] arrays. so: int elements[n][2]
  int *elements;
};

void jill_skiplist_init (struct jill_skiplist *self);
void jill_skiplist_term (struct jill_skiplist *self);

#endif
