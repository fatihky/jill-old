#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "utils/stopwatch.h"

#ifndef JILL_BENCH_INCLUDED
#define JILL_BENCH_INCLUDED

#define jill_bench_declare(cnt) \
  uint64_t jb_total; \
  uint64_t jb_thr; \
  struct jill_stopwatch jb_sw; \
  int jb_count = cnt

#define jill_bench_run(name, init_expr, term_expr, step_expr) do { \
  init_expr\
  jill_stopwatch_init (&jb_sw); \
  for (int i = 0; i < jb_count; i++) { \
    step_expr \
  } \
  jb_total = jill_stopwatch_term (&jb_sw); \
  term_expr \
  printf ("test: " name "\n"); \
  jb_thr = (uint64_t) ((double) jb_count / (double) jb_total * 1000000); \
  printf ("thr: %d ops/s\n", (int)jb_thr); \
} while(0)

#define jill_bench_thr() (jb_thr)
#define jill_bench_set_count(cnt) jb_count = cnt

#endif
