#ifndef JSON_H
#define JSON_H
#include <json-c/json.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"


/*Holds the price data*/
struct price_data
{
  double dk1[HOURS_USED];
  double dk2[HOURS_USED];
};


/*Extract the pricedata from the json object,
  returns a struct with two arrays(dk1 & dk2)*/
struct price_data extract_price_data(json_object *jso_today,
  json_object *jso_tomorrow);


/*Gets a child of jso specified by the format string and varargs*/
json_object *get_jso_from_format(json_object *jso, const char *format, ...);

/*Gets a child of jso array by index*/
json_object *get_jso_from_array_index(json_object *jso, size_t index);

/*Gets a child of jso by keys split by '.'*/
json_object *get_jso_from_keys(json_object *jso, const char *keys);


/*Prints a jso*/
void print_json_object(json_object *const json,
  const size_t indent,
  const int is_object);

#endif
