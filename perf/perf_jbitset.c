#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "storage/jbitset.h"
#include "utils/stopwatch.h"

int main(int argc, char *argv[]) {
  int count = 1000000;
  int rc;
  uint64_t total;
  uint64_t thr;
  struct jill_stopwatch sw;
  struct jill_bitset bs;
  if (argc > 1)
    count = atoi (argv[1]);

  #define test(name, val_expr) do { \
    rc = jill_bitset_init (&bs); \
    assert (rc == 0); \
    jill_stopwatch_init (&sw); \
    for (int i = 0; i < count; i++) { \
      rc = jill_bitset_add (&bs, val_expr); \
      assert (rc == 0); \
    } \
    total = jill_stopwatch_term (&sw); \
    jill_bitset_term (&bs); \
    printf ("test: " name "\n"); \
    thr = (uint64_t) ((double) count / (double) total * 1000000); \
    printf ("thr: %d ops/s\n", (int)thr); \
  } while(0)

  test("every bit is set", 1);
  test("every bit is off", 0);
  test("rand() %% 5 == 0", rand() % 5 == 0);

  return 0;
}
