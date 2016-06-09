#include "value.h"

#ifndef JILL_VALINDEX_H_INCLUDED
#define JILL_VALINDEX_H_INCLUDED

struct jill_valindex_base {
  /*  valindex's type. unique */
  int type;
  /*  create funcion. argument can be anything. for example, it can be sub-type
      of the index. must return NULL on error and set errno. */
  struct jill_valindex *(*create) (void *arg);
  /*  insert new element to the index. returns errno if error,
      zero on success. */
  int (*insert) (struct jill_valindex *self, struct jill_value *value);
  /*  set value of the nth element. returns EINVAL if index is not valid. */
  int (*set) (struct jill_valindex *self, int index,
    struct jill_value *value);
  /*  get value of the nth element. */
  int (*get) (struct jill_valindex *self, struct jill_value **value);
  /*  query the index */
  int (*query) (struct jill_valindex *self, void *query, void *result);
};

struct jill_valindex {
  struct jill_valindex_base *base;
  /*  custom fields can be placed here. */
};

/*  initialize valindex class globally. not thread-safe */
int jill_valindex_global_init();

/*  returns biggest internal struct jill_valindex type. */
int jill_valindex_last_internal_index();

/*  register new valindex type.
    some numbers are will be reserved(for example: from 0 to 4096) for jill's
    inernal use. and other numbers will not be restriced.
    returns 0 on success, errno on error. */
int jill_valindex_register (int type, struct jill_valindex_base *base);

/*  create new valindex with given type. */
struct jill_valindex *jill_valindex_create (int type, void *arg);

/*  insert new element to the index. returns errno if error,
    zero on success. */
int jill_valindex_insert (struct jill_valindex *self, struct jill_value *value);

/*  set value of the nth element. returns EINVAL if index is not valid. */
int jill_valindex_set (struct jill_valindex *self, int index,
  struct jill_value *value);

/*  get value of the nth element. */
int jill_valindex_get (struct jill_valindex *self, struct jill_value **value);

/*  query the index */
int jill_valindex_query (struct jill_valindex *self, void *query, void *result);

#endif
