#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_visit.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE ((size_t)512)

/*Finds the jso key*/
struct find_jso_key_userarg
{
  const char *key;
  json_object *value;
  size_t index;
};

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

/*Extract the pricedata from the json object,
  returns a struct with two arrays(dk1 & dk2)*/
struct price_data extract_price_data(json_object *jso);

json_c_visit_userfunc find_jso_key_visitor;

json_object *find_jso_key(json_object *jso, const char *key);

json_object *json_object_get_key(json_object *jso, const char *key);

json_object *get_from_index(json_object *jso, size_t index);

/*Converts string to double, changes , to .*/
double extract_price(const char *string);
