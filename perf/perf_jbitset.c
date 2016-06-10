#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "core/vallist.h"
#include "vallist/jbitset.h"
#include "bench.h"

int main(int argc, char *argv[]) {
  int count = 1000000;
  int rc;
  int ival;
  struct jill_vallist *vl;
  struct jill_value value;
  struct jill_value *valp = &value;

  value.valp = &ival;
  rc = jill_vallist_global_init();
  assert (rc == 0);

  if (argc > 1)
    count = atoi (argv[1]);

  jill_bench_declare(count);

  #define init { \
    vl = jill_vallist_create (JILL_VALLIST_BITSET, NULL); \
    assert (vl); \
  }

  #define term { \
    \
  }

  #define step(val_expr) { \
    rc = jill_vallist_insert (vl, val_expr ? &value : NULL); \
    assert (rc == 0); \
  }

  #define bench(name, val_expr) \
    jill_bench_run(name, init, term, step(val_expr))

  bench("every bit is set", 1);
  /*  validate */
  for (int i = 0; i < count; i++) {
    ival = 0;
    rc = jill_vallist_get (vl, i, &valp);
    assert (rc == 0);
    assert (ival == 1);
  }

  bench("every bit is off", 0);
  /*  validate */
  for (int i = 0; i < count; i++) {
    ival = 1;
    rc = jill_vallist_get (vl, i, &valp);
    assert (rc == 0);
    assert (ival == 0);
  }

  bench("rand() %% 5 == 0", rand() % 5 == 0);

  return 0;
}
