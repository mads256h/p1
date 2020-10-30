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
  char *file_content;
  json_object *jso;
  struct price_data prices;
  size_t i;

  file_content = read_file("real.json");
  /*Parses json, changes the string into an object*/
  jso = json_tokener_parse(file_content);
  free(file_content);

  /*If parsing failed*/
  if (!jso) {
    fprintf(stderr, "Parse failed\n");
    return EXIT_FAILURE;
  }


  /*Extracs pricedata from json*/
  prices = extract_price_data(jso);

  /*Frees up memory occupied by the jso object*/
  json_object_put(jso);

  printf("DK1:\n");
  for (i = 0; i < 24; i++) { printf("  %f\n", prices.dk1[i]); }

  printf("DK2:\n");
  for (i = 0; i < 24; i++) { printf("  %f\n", prices.dk2[i]); }

  /*Insert code here*/
  /* Rebuild */

  return EXIT_SUCCESS;
}
