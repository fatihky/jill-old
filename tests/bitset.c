#include <assert.h>
#include <stdlib.h>
#include "core/vallist.h"
#include "vallist/jbitset.h"

int main(int argc, char *argv[]) {
  int rc;
  int ival;
  struct jill_vallist *vl;
  struct jill_value value;
  struct jill_value *valuep = &value;
  rc = jill_vallist_global_init();
  assert (rc == 0);
  vl = jill_vallist_create (JILL_VALLIST_BITSET, NULL);
  assert (vl);
  /*  value of new bit. */
  rc = jill_vallist_insert (vl, &value);
  assert (rc == 0);
  value.valp = &ival;
  rc = jill_vallist_get (vl, 0, &valuep);
  assert (rc == 0);
  assert (ival == 1);
  return 0;
}
