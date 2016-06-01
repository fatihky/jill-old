#include <string.h>
#include <assert.h>
#include "storage/field.h"
#include "storage/vallist.h"

int main(int argc, char *argv[]) {
  int rc;
  struct jill_field field;
  jill_field_init (&field);
  rc = jill_field_set_name (&field, "test_field", strlen ("test_field"));
  assert (rc == 0);
  jill_field_term (&field);
  return 0;
}
