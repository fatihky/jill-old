#include "skiplist.h"

#ifndef JILL_PIECE_H_INCLUDED
#define JILL_PIECE_H_INCLUDED

struct jill_piece {
  int record_count;
  int timestamp_min_max[2];
  struct jill_skiplist skiplist;
};

void jill_piece_init (struct jill_piece *self);
void jill_piece_term (struct jill_piece *self);

#endif
