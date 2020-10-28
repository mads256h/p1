#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_visit.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE ((size_t)512)


struct price_data
{
  double dk1[24];
  double dk2[24];
};

/*Reads the file with the filename, returns a string with filecontents
  Remember to call free(free the memory)*/
char *read_file(const char *filename);

void print_json_object(json_object *const json,
  const size_t indent,
  const int is_object);

char *get_part(const char *string, int *found_dot);

json_object *get_jso_js_notation(json_object *jso, const char *string);

json_object *get_jso_from_format(json_object *jso, const char *format, ...);

/*Extract the pricedata from the json object,
  returns a struct with two arrays(dk1 & dk2)*/
struct price_data extract_price_data(json_object *jso);

json_object *get_jso_from_array_index(json_object *jso, size_t index);

/*Converts string to double, changes , to .*/
double string_to_double(const char *string);
