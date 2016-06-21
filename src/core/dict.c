#include <stdio.h>
#include <string.h>
#include "common.h"
#include "value.h"

static unsigned int hash (const void *key);
static void *keyDup (void *privdata, const void *key);
static void *valDup (void *privdata, const void *obj);
static int keyCompare (void *privdata, const void *key1, const void *key2);
static void keyDestructor (void *privdata, void *key);
static void valDestructor(void *privdata, void *obj);

dictType dictTypeJillValToPtr = {
  hash,
  keyDup,
  valDup,
  keyCompare,
  keyDestructor,
  valDestructor
};

static unsigned int hash (const void *key) {
  struct jill_value *val = (struct jill_value *)key;
  return dictGenCaseHashFunction (val->valp, *(int *)val->lenp);
}

static void *keyDup (void *privdata, const void *key) {
  /*
    here is the structure of allocated memory
    [ block allocated with one alloc ###################### ]
    [struct jill_value] [int(lenp points here)]
                    [byte array. value buffer copied to here]
  */
  (void) privdata;
  struct jill_value *val = (struct jill_value *)key;
  char *ptr;
  size_t allocsz = sizeof (struct jill_value) + // struct
                   sizeof (int) + // len
                   (*(int *)val->lenp); // value buffer
  struct jill_value *dup = zmalloc (allocsz);
  if (!dup)
    return NULL;
  ptr = (char *)dup;
  // move pointer by size of struct jill_value
  ptr += (sizeof (struct jill_value));
  // copy length from value
  *(int *)ptr = *(int *)val->lenp;
  // set length pointer's value
  dup->lenp = ptr;
  // move pointer by size of length
  ptr += sizeof(int);
  // set value pointer's value
  dup->valp = ptr;
  // copy value buffer to the new duplicate value
  memcpy (dup->valp, val->valp, (size_t) (*(int *)val->lenp));
  return dup;
}

static void *valDup (void *privdata, const void *obj) {
  (void) privdata;
  return (void *)obj;
}

static int keyCompare (void *privdata, const void *key1, const void *key2) {
  (void) privdata;
  const struct jill_value *v1 = key1, *v2 = key2;
  int l1 = *(int *)v1->lenp;
  int l2 = *(int *)v2->lenp;

  if (l1 != l2)
    return 0;

  return memcmp (v1->valp, v2->valp, l1) == 0;
}

static void keyDestructor (void *privdata, void *key) {
  // since we are allocating duplicate keys with just one allocation,
  // this is enough
  (void) privdata;
  zfree (key);
}

static void valDestructor(void *privdata, void *obj) {
  // values are just pointers so we will not do anything special.
  (void) privdata;
  (void) obj;
}
