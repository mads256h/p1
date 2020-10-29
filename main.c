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

  char *file_content = read_file("real.json");
  /*Parses json, changes the string into an object*/
  json_object *jso = json_tokener_parse(file_content);
  free(file_content);

  /*If parsing failed*/
  if (!jso) {
    fprintf(stderr, "Parse failed\n");
    return EXIT_FAILURE;
  }


  /*Extracs pricedata from json*/
  struct price_data data = extract_price_data(jso);

  printf("DK1:\n");
  for (int i = 0; i < 24; i++) { printf("  %f\n", data.dk1[i]); }

  printf("DK2:\n");
  for (int i = 0; i < 24; i++) { printf("  %f\n", data.dk2[i]); }

  /*Insert code here*/
  /* Rebuild */

  /*Frees up memory occupied by the jso object*/
  json_object_put(jso);
  return EXIT_SUCCESS;
}
