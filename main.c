#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_object_iterator.h>
#include <json-c/json_tokener.h>
#include <json-c/json_visit.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

int main(void)
{
  char *buffer = read_json("real.json");
  json_object *test_json = json_tokener_parse(buffer);
  free(buffer);

  if (!test_json) {
    fprintf(stderr, "Parse failed\n");
    return EXIT_FAILURE;
  }

  struct price_data data = extract_data(test_json);

  printf("DK1:\n");
  for (int i = 0; i < 24; i++) { printf("  %f\n", data.dk1[i]); }

  printf("DK2:\n");
  for (int i = 0; i < 24; i++) { printf("  %f\n", data.dk2[i]); }

  /*Insert code here*/
  /* Rebuild */

  json_object_put(test_json);
  return EXIT_SUCCESS;
}
