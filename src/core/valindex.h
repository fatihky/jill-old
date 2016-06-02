#ifndef JILL_VALINDEX_H_INCLUDED
#define JILL_VALINDEX_H_INCLUDED

struct jill_valindex {
  // for example, a bitmap can be placed here
};

void jill_valindex_init (struct jill_valindex *self);
void jill_valindex_term (struct jill_valindex *self);

#endif
