#include <assert.h>
#include "storage/vallist.h"

int main(int argc, char *argv[]) {
  int rc;
  int val = 274;
  int len = sizeof(int);
  struct jill_vallist vl;
  // fixed-length value list
  jill_vallist_init (&vl);
  jill_vallist_set_fixed (&vl, 4);
  jill_vallist_set_grow (&vl, 1);
  rc = jill_vallist_add_fixed (&vl, &val);
  assert (rc == 0);
  assert (((int *)vl.fixed_size_vals.elements)[0] == val);
  val += 236;
  rc = jill_vallist_add_fixed (&vl, &val);
  assert (rc == 0);
  assert (((int *)vl.fixed_size_vals.elements)[1] == val);
  jill_vallist_term(&vl);
  // length-prefixed value list
  jill_vallist_init (&vl);
  jill_vallist_set_length_prefixed (&vl, sizeof (int));
  jill_vallist_set_grow (&vl, 1);
  rc = jill_vallist_add_length_prefixed (&vl, &len, &val, sizeof (val));
  assert (rc == 0);
  assert (((int *)vl.length_prefixed_vals.lengths)[0] == len);
  assert (((int *)vl.length_prefixed_vals.value_buffer)[0] == val);
  jill_vallist_term(&vl);
  return 0;
}
