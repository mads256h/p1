#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_visit.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE ((size_t)512)

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

char *read_json(const char *filename);

void print_json_object(json_object *const json,
  const size_t indent,
  const int is_object);

struct price_data extract_data(json_object *jso);

json_c_visit_userfunc find_jso_key_visitor;

json_object *find_jso_key(json_object *jso, const char *key);

json_object *json_object_get_key(json_object *jso, const char *key);

json_object *get_from_index(json_object *jso, size_t index);

double extract_price(const char *string);
