#include <curl/curl.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_object_iterator.h>
#include <json-c/json_tokener.h>
#include <json-c/json_visit.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "json.h"
#include "math.h"
#include "util.h"
#include "www.h"


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
  const char url_part[] =
    "https://www.nordpoolgroup.com/api/marketdata/page/"
    "41?currency=,DKK,DKK,DKK&endDate=";
  struct tm tdy;
  struct tm tmw;
  char *url;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  tdy = *today();
  tmw = *tomorrow();


  /* Today */
  url = format_url(url_part, tdy);
  printf("%s\n", url);
  file_content = download_url(url);
  free(url);
  /*Parses json, changes the string into an object*/
  jso_today = json_tokener_parse(file_content);
  free(file_content);

  /* Tomorrow */
  url = format_url(url_part, tmw);
  printf("%s\n", url);
  file_content = download_url(url);
  free(url);
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
  print_prices(tdy, tmw, prices.dk1);

  printf("DK2:\n");
  print_prices(tdy, tmw, prices.dk2);


  /*Insert code here*/
  /* Rebuild */
  printf("\nCheapest DK1:\n");
  cheapest_indecies =
    find_cheapest_hours((size_t)0, (size_t)HOURS_USED, 0.5, prices.dk1, &hours);

  selected_price = malloc(hours * sizeof(double));
  printf("Charge time: %d hours\n", (int)hours);

  print_cheapest_prices(tdy, tmw, prices.dk1, cheapest_indecies, hours);

  for (i = 0; i < hours; i++) {
    selected_price[i] = prices.dk1[cheapest_indecies[i]];
  }

  free(cheapest_indecies);

  prices_average =
    average(prices.dk1, sizeof(prices.dk1) / sizeof(prices.dk1[0]));
  cheapest_average = average(selected_price, hours);

  free(selected_price);


  printf("Saved: %.2f%%\n", (1.0 - cheapest_average / prices_average) * 100.0);

  curl_global_cleanup();

  return EXIT_SUCCESS;
}
