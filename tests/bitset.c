#include <assert.h>
#include "storage/jbitset.h"

int main(int argc, char *argv[]) {
  int rc;
  struct jill_bitset bs;
  rc = jill_bitset_init (&bs);
  assert (rc == 0);
  /*  value of new bit. */
  rc = jill_bitset_add (&bs, 1);
  assert (rc == 0);
  assert(jill_bitset_get (&bs, 0));
  jill_bitset_term (&bs);
  return 0;
}
