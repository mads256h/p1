#if NDEBUG
#undef NDEBUG
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "json.h"


int main(void)
{

  assert(extract_price("0") == 0.0);
  assert(extract_price("1") == 1.0);
  assert(extract_price("11.02") == 11.02);
  assert(extract_price("11,02") == 11.02);
  assert(extract_price("-20.12") == -20.12);
  assert(extract_price("-20,12") == -20.12);

  return EXIT_SUCCESS;
}
