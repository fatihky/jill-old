#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "core/common.h"
#include "core/value.h"

int main(int argc, char *argv[]) {
  int rc;
  int len;
  struct jill_value val;
  char *testVal = "testval";
  dictEntry *de;
  val.lenp = &len;
  dict *dt = dictCreate (&dictTypeJillValToPtr, NULL);
  assert (dt);
  val.valp = "testkey";
  len = 7;
  rc = dictAdd(dt, &val, testVal);
  assert (rc == 0);
  de = dictFind(dt, (const void *)&val);
  assert (de);
  assert (de->v.val == testVal);
  /*  there is no need to result's value. because it is exactly what we set */
  // assert (memcmp (de->v.val, testVal, len) == 0);
  return 0;
}
