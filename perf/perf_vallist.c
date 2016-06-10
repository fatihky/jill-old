
#include "core/vallist.h"
#include "bench.h"

#define GROW 10

int main(int argc, char *argv[]) {

#if 0
  int count = 1000000;
  int rc;
  int fixedval = 274;
  int len = 6;
  char *strval = "_jill_";
  struct jill_vallist vl;

  if (argc > 1)
    count = atoi (argv[1]);

  jill_bench_declare(count);

  jill_bench_run("fixed size values", {
    jill_vallist_init(&vl);
    jill_vallist_set_grow(&vl, GROW);
    jill_vallist_set_fixed (&vl, 4);
  }, {
    jill_vallist_term(&vl);
  }, {
    rc = jill_vallist_add_fixed (&vl, &fixedval);
    assert (rc == 0);
  });

  jill_bench_run("length prefixed values", {
    jill_vallist_init(&vl);
    jill_vallist_set_grow(&vl, GROW);
    jill_vallist_set_length_prefixed (&vl, 4);
  }, {
    jill_vallist_term(&vl);
  }, {
    rc = jill_vallist_add_length_prefixed (&vl, &len, strval, (size_t)len);
    assert (rc == 0);
  });

  jill_bench_run("length prefixed values - preallocate", {
    jill_vallist_init(&vl);
    jill_vallist_set_grow(&vl, GROW);
    jill_vallist_set_length_prefixed (&vl, 4);
    jill_vallist_lp_prealloc (&vl, len * count);
  }, {
    jill_vallist_term(&vl);
  }, {
    rc = jill_vallist_add_length_prefixed (&vl, &len, strval, (size_t)len);
    assert (rc == 0);
  });

#endif

  return 0;
}
