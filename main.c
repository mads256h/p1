#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_object_iterator.h>
#include <json-c/json_tokener.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

char *read_json(const char *filename);

void print_json_object(json_object *const json,
  const size_t indent,
  const int is_object);

int main(void)
{
  char *buffer = read_json("real.json");
  json_object *test_json = json_tokener_parse(buffer);
  free(buffer);

  if (!test_json) {
    fprintf(stderr, "Parse failed\n");
    return EXIT_FAILURE;
  }

  print_json_object(test_json, (size_t)0, 0);
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
