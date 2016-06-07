#ifndef JILL_VALUE_INCLUDED
#define JILL_VALUE_INCLUDED

struct jill_value {
  void *lenp;
  void *valp;
  size_t vallen;
};

#endif
