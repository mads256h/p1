#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_object_iterator.h>
#include <json-c/json_tokener.h>
#include <stdio.h>
#include <stdlib.h>

void print_json_object(json_object *json, int indent, int is_object);

int main(void)
{
  json_object *test_json = json_tokener_parse(
    "{\"test\":\"hello\", \"json\": \"sucks\", \"int\": 123, \"double\": "
    "0.123, \"object\": {\"int1\": 1, \"int2\": 2, \"int3\": 3, \"int4\": 4, "
    "\"int5\": 5, \"int6\": 6, \"object2\": {\"bruh\": \"moment\"}}, "
    "\"array\": [1, 2, 3, 4, 5, 6, 7, 8, 9], "
    "\"null\": null, \"boolean\": true}");

  if (!test_json) {
    fprintf(stderr, "Parse failed\n");
    return EXIT_FAILURE;
  }

  print_json_object(test_json, 0, 0);
  json_object_put(test_json);
  return EXIT_SUCCESS;
}

void print_json_object(json_object *json, int indent, int is_object)
{
  int i;
  enum json_type type;

  type = json_object_get_type(json);

  if (!is_object && type != json_type_array && type != json_type_object) {
    for (i = 0; i < indent * 2; i++) { printf(" "); }
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
      for (i = 0; i < (indent + 1) * 2; i++) { printf(" "); }
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
