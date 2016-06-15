#include "value.h"

#ifndef JILL_VALLIST_H_INCLUDED
#define JILL_VALLIST_H_INCLUDED

struct jill_vallist_base {
  /*  vallist's type. unique */
  int type;
  /*  create funcion. argument can be anything. for example, it can be sub-type
      of the index. must return NULL on error and set errno. */
  struct jill_vallist *(*create) (void *arg);
  /*  free allocated memory for vallist */
  void (*destroy) (struct jill_vallist *self);
  /*  set option */
  int (*setopt) (struct jill_vallist *self, int option, const void *optval,
    size_t optvallen);
  /*  get option */
  int (*getopt) (struct jill_vallist *self, int option, void *optval,
    size_t *optvallen);
  /*  call custom method */
  int (*run_custom_method) (struct jill_vallist *self, int method, void *arg,
    void *result);
  /*  insert new element to the index. returns errno if error,
      zero on success. */
  int (*insert) (struct jill_vallist *self, struct jill_value *value);
  /*  set value of the nth element. returns EINVAL if index is not valid. */
  int (*set) (struct jill_vallist *self, int index,
    struct jill_value *value);
  /*  get value of the nth element. */
  int (*get) (struct jill_vallist *self, int index, struct jill_value **value);
  /*  query the index */
  int (*query) (struct jill_vallist *self, void *query, void *result);
};

struct jill_vallist {
  struct jill_vallist_base *base;
  /*  custom fields can be placed here. */
};

/*  initialize vallist class globally. not thread-safe */
int jill_vallist_global_init();

/*  returns biggest internal struct jill_vallist type. */
int jill_vallist_last_internal_index();

/*  register new vallist type.
    some numbers are will be reserved(for example: from 0 to 4096) for jill's
    inernal use. and other numbers will not be restriced.
    returns 0 on success, errno on error. */
int jill_vallist_register (int type, struct jill_vallist_base *base);

/*  create new vallist with given type. */
struct jill_vallist *jill_vallist_create (int type, void *arg);

/*  insert new element to the index. returns errno if error,
    zero on success. */
int jill_vallist_insert (struct jill_vallist *self, struct jill_value *value);

/*  set value of the nth element. returns EINVAL if index is not valid. */
int jill_vallist_set (struct jill_vallist *self, int index,
  struct jill_value *value);

/*  get value of the nth element. */
int jill_vallist_get (struct jill_vallist *self, int index,
  struct jill_value **value);

/*  query the index */
int jill_vallist_query (struct jill_vallist *self, void *query, void *result);

/*  destroy the index */
void jill_vallist_destroy (struct jill_vallist *self);

#endif
