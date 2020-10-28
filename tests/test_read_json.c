#if NDEBUG
#undef NDEBUG
#endif


#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"


const char *str =
  "{\n"
  "  \"json\" : \"test\"\n"
  "}\n";

const char *windows_str =
  "{\r\n"
  "  \"json\" : \"test\"\r\n"
  "}\r\n";

int main(void)
{
  char *file_content = read_json("test_read_json.json");

  assert(file_content);

  assert(
    strcmp(file_content, str) == 0 || strcmp(file_content, windows_str) == 0);

  return EXIT_SUCCESS;
}
