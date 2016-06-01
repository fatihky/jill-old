#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

#include "stopwatch.h"

/*  Check whether measured time is the expected time (in microseconds).
    The upper tolerance is 50ms so that the test doesn't fail even on
    very slow or very loaded systems.  Likewise on some systems we can
    wind up firing up to a single tick early (Windows), so the lower bound
    is pretty low.  The consequence of this is that programs which  specify
    a timeout should be a little more pessimistic (at least 10ms) then they
    might otherwise think they need to be. */
#define time_assert(actual,expected) \
    assert (actual > ((expected) - 10000) && actual < ((expected) + 50000));

void jill_stopwatch_init (struct jill_stopwatch *self) {
  int rc;
  struct timeval tv;

  rc = gettimeofday (&tv, NULL);
  assert (rc == 0);
  self->start = (uint64_t) (((uint64_t) tv.tv_sec) * 1000000 + tv.tv_usec);
}

uint64_t jill_stopwatch_term (struct jill_stopwatch *self) {
  int rc;
  struct timeval tv;
  uint64_t end;

  rc = gettimeofday (&tv, NULL);
  assert (rc == 0);
  end = (uint64_t) (((uint64_t) tv.tv_sec) * 1000000 + tv.tv_usec);
  return end - self->start;
}