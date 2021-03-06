#include "vallist.h"

#ifndef JILL_FIELD_H_INCLUDED
#define JILL_FIELD_H_INCLUDED

struct jill_field {
  char *name;
};

int jill_field_init (struct jill_field *self);
int jill_field_set_name (struct jill_field *self, char *name, size_t namelen);
void jill_field_term (struct jill_field *self);

#endif
