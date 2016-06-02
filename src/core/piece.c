#include "piece.h"

void jill_piece_init (struct jill_piece *self) {
  self->record_count = 0;
  self->timestamp_min_max[0] = -1;
  self->timestamp_min_max[1] = -1;
}

void jill_piece_term (struct jill_piece *self) {
}