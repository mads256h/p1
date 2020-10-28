#include <assert.h>
#include <string.h>

#include "json.h"

char *read_file(const char *filename)
{
  int c;
  size_t i = 0, cur_size = BUFFER_SIZE;
  char *buffer = malloc(BUFFER_SIZE);
  assert(buffer);

  FILE *file = fopen(filename, "rb");
  assert(file);

  while ((c = getc(file)) != EOF) {
    if ((i + 1) > cur_size) {
      buffer = realloc(buffer, cur_size += BUFFER_SIZE);
      assert(buffer);
    }
    buffer[i] = (char)c;
    i++;
  }
  buffer[i] = 0;
  fclose(file);

  assert(i <= cur_size);
  return buffer;
}


struct price_data extract_price_data(json_object *jso)
{
  struct price_data ret;

  json_object *rows_jso = get_jso_js_notation(jso, "data.Rows");

  for (size_t i = 0; i < 24; i++) {

    json_object *first_jso = get_from_index(rows_jso, i);
    json_object *columns_jso = get_jso_js_notation(first_jso, "Columns");

    for (size_t j = 0; j < 2; j++) {
      json_object *column_jso = get_from_index(columns_jso, j);
      json_object *column_value_jso = get_jso_js_notation(column_jso, "Value");

      if (j == 0) {
        ret.dk1[i] = string_to_double(json_object_get_string(column_value_jso));
      } else {
        ret.dk2[i] = string_to_double(json_object_get_string(column_value_jso));
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

char *get_part(const char *string, int *found_dot)
{
  assert(string);
  assert(found_dot);


  char *dup = strdup(string);

  assert(dup);

  size_t str_len = strlen(dup);

  *found_dot = 0;

  for (size_t i = 0; i < str_len; i++) {
    if (dup[i] == '.') {
      dup[i] = 0;
      *found_dot = 1;
      break;
    }
  }

  return dup;
}

json_object *get_jso_js_notation(json_object *jso, const char *string)
{
  assert(jso);
  assert(string);


  const char *cur_string = string;
  json_object *cur_jso = jso;

  while (strlen(cur_string) > 0) {

    int found_dot = 0;
    char *part = get_part(cur_string, &found_dot);
    json_object_object_foreach(cur_jso, key, val)
    {
      if (strcmp(part, key) == 0) {
        cur_jso = val;
        break;
      }
    }
    cur_string += strlen(part) + (size_t)found_dot;


    free(part);
  }

  assert(strlen(string) == (size_t)cur_string - (size_t)string);
  return cur_jso;
}


json_object *get_from_index(json_object *jso, size_t index)
{
  assert(jso);

  assert(json_object_get_type(jso) == json_type_array);
  assert(index < json_object_array_length(jso));

  return json_object_array_get_idx(jso, index);
}

double string_to_double(const char *string)
{
  assert(string);

  char *dup = strdup(string);

  assert(dup);

  size_t str_len = strlen(dup);

  for (size_t i = 0; i < str_len; i++) {
    if (dup[i] == ',') { dup[i] = '.'; }
  }

  double ret = strtod(dup, 0);

  free(dup);

  return ret;
}
