#if NDEBUG
#undef NDEBUG
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "json.h"


int main(void)
{

  assert(string_to_double("0") == 0.0);
  assert(string_to_double("1") == 1.0);
  assert(string_to_double("11.02") == 11.02);
  assert(string_to_double("11,02") == 11.02);
  assert(string_to_double("-20.12") == -20.12);
  assert(string_to_double("-20,12") == -20.12);

  return EXIT_SUCCESS;
}
