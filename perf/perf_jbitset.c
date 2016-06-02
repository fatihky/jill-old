#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "core/jbitset.h"
#include "bench.h"

int main(int argc, char *argv[]) {
  int count = 1000000;
  int rc;
  struct jill_bitset bs;

  if (argc > 1)
    count = atoi (argv[1]);

  jill_bench_declare(count);

  #define init { \
    rc = jill_bitset_init (&bs); \
    assert (rc == 0); \
  }

  #define term { \
    jill_bitset_term (&bs); \
  }

  #define step(val_expr) { \
    rc = jill_bitset_add (&bs, val_expr); \
    assert (rc == 0); \
  }

  #define bench(name, val_expr) \
    jill_bench_run(name, init, term, step(val_expr))

  bench("every bit is set", 1);
  bench("every bit is off", 0);
  bench("rand() %% 5 == 0", rand() % 5 == 0);

  return 0;
}
