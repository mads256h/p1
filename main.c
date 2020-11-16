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
#include "math.h"
#include "util.h"


int main(void)
{
  char *file_content;
  json_object *jso_today;
  json_object *jso_tomorrow;
  struct price_data prices;
  size_t i, hours;
  size_t *cheapest_indecies;
  double *selected_price;
  double prices_average;
  double cheapest_average;

  /* Today */
  file_content = read_file("today.json");
  /*Parses json, changes the string into an object*/
  jso_today = json_tokener_parse(file_content);
  free(file_content);

  /* Tomorrow */
  file_content = read_file("tomorrow.json");
  jso_tomorrow = json_tokener_parse(file_content);
  free(file_content);

  /*If parsing failed*/
  if (!jso_today || !jso_tomorrow) {
    fprintf(stderr, "Parse failed\n");
    return EXIT_FAILURE;
  }


  /*Extracs pricedata from json*/
  prices = extract_price_data(jso_today, jso_tomorrow);


  /*Frees up memory occupied by the jso object*/
  json_object_put(jso_today);
  json_object_put(jso_tomorrow);

  printf("DK1:\n");
  for (i = 0; i < HOURS_USED; i++) {
    printf("%d  %f\n", (int)i, prices.dk1[i]);
  }

  printf("DK2:\n");
  for (i = 0; i < HOURS_USED; i++) {
    printf("%d  %f\n", (int)i, prices.dk2[i]);
  }

  /*Insert code here*/
  /* Rebuild */
  printf("\n Cheapest: \n");
  cheapest_indecies =
    find_cheapest_hours((size_t)0, (size_t)HOURS_USED, 0.0, prices.dk1, &hours);

  selected_price = malloc(hours * sizeof(double));
  printf("Size: %d\n", (int)hours);

  for (i = 0; i < hours; i++) {
    printf("%f\n", prices.dk1[cheapest_indecies[i]]);
  }

  for (i = 0; i < hours; i++) {
    selected_price[i] = prices.dk1[cheapest_indecies[i]];
  }

  free(cheapest_indecies);

  prices_average =
    average(prices.dk1, sizeof(prices.dk1) / sizeof(prices.dk1[0]));
  cheapest_average = average(selected_price, hours);

  free(selected_price);


  printf("Saved: %f%%\n", (1.0 - cheapest_average / prices_average) * 100.0);

  return EXIT_SUCCESS;
}
