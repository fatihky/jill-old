#include <assert.h>
#include "core/vallist.h"
#include "vallist/fixed.h"
#include "vallist/len_prefixed.h"

int main(int argc, char *argv[]) {
  int rc;
  int subtype;
  int val;
  int len;
  struct jill_vallist *vl;
  struct jill_value value;
  struct jill_value *valp = &value;
  value.valp = &val;
  value.lenp = &len;

  rc = jill_vallist_global_init();
  assert (rc == 0);
  {
    // text fixed length
    subtype = JILL_VALLIST_FIXED_I32;
    vl = jill_vallist_create (JILL_VALLIST_FIXED, &subtype);
    assert (vl);
    /*  add value */
    /*  add value 1 */
    val = 274;
    rc = jill_vallist_insert (vl, &value);
    assert (rc == 0);
    val = -1;
    rc = jill_vallist_get (vl, 0, &valp);
    assert (rc == 0);
    assert (val == 274);
    /*  add value 2 */
    val = 236;
    rc = jill_vallist_insert (vl, &value);
    assert (rc == 0);
    val = -1;
    rc = jill_vallist_get (vl, 1, &valp);
    assert (rc == 0);
    assert (val == 236);
    jill_vallist_destroy (vl);
  }

  {
    // length-prefixed vallist tests
    subtype = JILL_VALLIST_LENGTH_PREFIXED_I32;
    vl = jill_vallist_create (JILL_VALLIST_LENGTH_PREFIXED, &subtype);
    assert (vl);
    len = 5;
    value.valp = "fatih";
    rc = jill_vallist_insert (vl, &value);
    assert (rc == 0);
    rc = jill_vallist_get (vl, 0, &valp);
    assert (rc == 0);
    assert (*(int *)value.lenp == len);
    jill_vallist_destroy (vl);
  }

  return 0;
}
