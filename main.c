#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_object_iterator.h>
#include <json-c/json_tokener.h>
#include <json-c/json_visit.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

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

  json_object_put(test_json);
  return EXIT_SUCCESS;
}

char *read_json(const char *filename)
{
  int c, i = 0, cur_size = BUFFER_SIZE;
  char *buffer = malloc(BUFFER_SIZE);

  FILE *file = fopen(filename, "r");
  while ((c = getc(file)) != EOF) {
    if ((i + 1) > cur_size) {
      buffer = realloc(buffer, cur_size += BUFFER_SIZE);
    }
    buffer[i] = c;
    i++;
  }
  buffer[i] = 0;
  fclose(file);
  printf("Current size: %d, i = %d\n", cur_size, i);
  return buffer;
}


struct price_data extract_data(json_object *jso)
{
  struct price_data ret;

  json_object *rows_jso = find_jso_key(jso, "Rows");

  for (int i = 0; i < 24; i++) {

    json_object *first_jso = get_from_index(rows_jso, (size_t)i);
    json_object *columns_jso = find_jso_key(first_jso, "Columns");

    for (int j = 0; j < 2; j++) {
      json_object *column_jso = get_from_index(columns_jso, j);
      json_object *column_value_jso = json_object_get_key(column_jso, "Value");

      if (j == 0) {
        ret.dk1[i] = extract_price(json_object_get_string(column_value_jso));
      } else {
        ret.dk2[i] = extract_price(json_object_get_string(column_value_jso));
      }
    }
  }

  return ret;
}

void print_json_object(json_object *const json,
  const size_t indent,
  const int is_object)
{
  size_t i;
  enum json_type type;

  type = json_object_get_type(json);

  if (!is_object && type != json_type_array && type != json_type_object) {
    for (i = 0; i < indent; i++) { printf("  "); }
  }

  switch (type) {
  case json_type_null:
    printf("null (json_type_null)");
    break;
  case json_type_boolean:
    printf("%s (json_type_boolean)",
      json_object_get_boolean(json) ? "true" : "false");
    break;
  case json_type_double:
    printf("%f (json_type_double)", json_object_get_double(json));
    break;
  case json_type_int:
    printf("%d (json_type_int)", json_object_get_int(json));
    break;
  case json_type_object:
    printf("(json_type_object)\n");
    json_object_object_foreach(json, key, val)
    {
      for (i = 0; i < indent + 1; i++) { printf("  "); }
      printf("%s: ", key);
      print_json_object(val, indent + 1, 1);
    }
    return;
  case json_type_array:
    printf("(json_type_array)\n");
    for (i = 0; i < json_object_array_length(json); i++) {
      print_json_object(json_object_array_get_idx(json, i), indent + 1, 0);
    }
    return;
  case json_type_string:
    printf("%s (json_type_string)", json_object_get_string(json));
    break;
  default:
    printf("unknown");
    break;
  }

  printf("\n");
}

int find_jso_key_visitor(json_object *jso,
  int flags,
  json_object *parent_jso,
  const char *jso_key,
  size_t *jso_index,
  void *userarg)
{
  struct find_jso_key_userarg *find_jso_struct = userarg;


  if (jso_key && strcmp(jso_key, find_jso_struct->key) == 0) {

    find_jso_struct->value = jso;
    find_jso_struct->index = jso_index ? *jso_index : SIZE_MAX;

    return JSON_C_VISIT_RETURN_STOP;
  }
  return JSON_C_VISIT_RETURN_CONTINUE;
}

json_object *find_jso_key(json_object *jso, const char *key)
{
  struct find_jso_key_userarg find_jso_struct;
  find_jso_struct.key = key;
  find_jso_struct.value = 0;
  find_jso_struct.index = SIZE_MAX;

  json_c_visit(jso, 0, find_jso_key_visitor, &find_jso_struct);

  return find_jso_struct.value;
}


json_object *json_object_get_key(json_object *jso, const char *key)
{
  json_object_object_foreach(jso, jso_key, jso_val)
  {
    if (strcmp(key, jso_key) == 0) { return jso_val; }
  }

  return 0;
}


json_object *get_from_index(json_object *jso, size_t index)
{
  return json_object_array_get_idx(jso, index);
}

double extract_price(const char *string)
{
  char *dup = strdup(string);

  size_t str_len = strlen(dup);

  for (size_t i = 0; i < str_len; i++) {
    if (dup[i] == ',') { dup[i] = '.'; }
  }

  double ret = strtod(dup, 0);

  free(dup);

  return ret;
}
