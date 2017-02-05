#include <assert.h>
#include "core/common.h"
#include "core/vallist.h"
#include "vallist/common.h"

int main (int argc, char *argv[]) {
  int rc;
  struct jill_vallist *htags;
  rc = jill_vallist_global_init();
  assert (rc == 0);
  htags = jill_vallist_create (JILL_VALLIST_DISTINCT, NULL);
  assert (htags != NULL);
  return 0;
}
