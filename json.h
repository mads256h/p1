#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_visit.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"


#define BUFFER_SIZE ((size_t)512)


/*Holds the price data*/
struct price_data
{
  double dk1[HOURS_USED];
  double dk2[HOURS_USED];
};

/*Holds the strings split data*/
struct split_string_data
{
  size_t size;
  char **strings;
};


/*Reads the file with the filename, returns true if successful
  Remember to call free(free the memory)*/
int read_file(const char *filename, /* out */ char **content);


/*Extract the pricedata from the json object,
  returns a struct with two arrays(dk1 & dk2)*/
struct price_data extract_price_data(json_object *jso_today,
  json_object *jso_tomorrow);


/*Gets a child of jso specified by the format string and varargs*/
json_object *get_jso_from_format(json_object *jso, const char *format, ...);


/*Converts string to double, changes , to .*/
double string_to_double(const char *string);


/*Gets a child of jso array by index*/
json_object *get_jso_from_array_index(json_object *jso, size_t index);


/*Gets a child of jso by keys split by '.'*/
json_object *get_jso_from_keys(json_object *jso, const char *keys);


/*Splits a string by 'split_char'*/
struct split_string_data split_string(const char *string,
  const char split_char);

/*Frees the struct returned from split_string*/
void free_split_string(struct split_string_data data);


/*Prints a jso*/
void print_json_object(json_object *const json,
  const size_t indent,
  const int is_object);
