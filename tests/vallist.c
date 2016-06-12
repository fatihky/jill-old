#include <assert.h>
#include "core/vallist.h"
#include "vallist/fixed.h"

int main(int argc, char *argv[]) {
  int rc;
  int subtype;
  int val;
  struct jill_vallist *vl;
  struct jill_value value;

  rc = jill_vallist_global_init();
  assert (rc == 0);
  {
    // text fixed length
    subtype = JILL_VALLIST_FIXED_I32;
    vl = jill_vallist_create (JILL_VALLIST_FIXED, &subtype);
    assert (vl);
    /*  add value */
    val = 274;
    value.valp = &val;
    rc = jill_vallist_insert (vl, &value);
    assert (rc == 0);
    jill_vallist_destroy (vl);
  }
#if 0
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
  rc = jill_vallist_lp_prealloc (&vl, 100);
  assert (rc == 0);
  rc = jill_vallist_add_length_prefixed (&vl, &len, &val, sizeof (val));
  assert (rc == 0);
  assert (((int *)vl.length_prefixed_vals.lengths)[0] == len);
  assert (((int *)vl.length_prefixed_vals.value_buffer)[0] == val);
  jill_vallist_term(&vl);
#endif
  return 0;
}
