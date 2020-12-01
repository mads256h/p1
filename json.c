#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "json.h"


struct price_data extract_price_data(json_object *const jso_today,
  json_object *const jso_tomorrow)
{
  size_t i;
  json_object *value;
  struct price_data prices;

  for (i = 0; i < HOURS_USED / 2; i++) {
    /* equivalent to data.Rows[i].Columns[0].Value */
    value = get_jso_from_format(
      jso_today, "kikik", "data.Rows", i, "Columns", 0, "Value");
    prices.dk1[i] = string_to_double(json_object_get_string(value));

    /* equivalent to data.Rows[i].Columns[1].Value */
    value = get_jso_from_format(
      jso_today, "kikik", "data.Rows", i, "Columns", 1, "Value");
    prices.dk2[i] = string_to_double(json_object_get_string(value));
  }

  for (i = HOURS_USED / 2; i < HOURS_USED; i++) {
    value = get_jso_from_format(jso_tomorrow,
      "kikik",
      "data.Rows",
      i - HOURS_USED / 2,
      "Columns",
      0,
      "Value");
    prices.dk1[i] = string_to_double(json_object_get_string(value));

    /* equivalent to data.Rows[i].Columns[1].Value */
    value = get_jso_from_format(jso_tomorrow,
      "kikik",
      "data.Rows",
      i - HOURS_USED / 2,
      "Columns",
      1,
      "Value");
    prices.dk2[i] = string_to_double(json_object_get_string(value));
  }

  return prices;
}


json_object *
  get_jso_from_format(json_object *const jso, const char *const format, ...)
{
  va_list ap;
  size_t i, str_len;
  json_object *cur_jso;
  size_t index_test;
  const char *string_test;
  assert(jso);
  assert(format);

  cur_jso = jso;

  str_len = strlen(format);

  va_start(ap, format);

  for (i = 0; i < str_len; i++) {
    if (format[i] == 'i') {
      index_test = va_arg(ap, size_t);
      cur_jso = get_jso_from_array_index(cur_jso, index_test);
    } else if (format[i] == 'k') {
      string_test = va_arg(ap, const char *);
      cur_jso = get_jso_from_keys(cur_jso, string_test);
    } else {
      assert(0);
    }

    assert(cur_jso);
  }

  va_end(ap);


  return cur_jso;
}

json_object *get_jso_from_array_index(json_object *const jso,
  const size_t index)
{
  assert(jso);

  assert(json_object_get_type(jso) == json_type_array);

  assert(index < json_object_array_length(jso));

  return json_object_array_get_idx(jso, index);
}


json_object *get_jso_from_keys(json_object *const jso, const char *const keys)
{
  json_object *cur_jso;
  struct split_string_data split;
  size_t i;

  assert(jso);
  assert(keys);

  cur_jso = jso;
  split = split_string(keys, '.');


  for (i = 0; i < split.size; i++) {
    cur_jso = json_object_object_get(cur_jso, split.strings[i]);

    assert(cur_jso);
  }

  free_split_string(split);

  return cur_jso;
}

void print_json_object(json_object *const json,
  const size_t indent,
  const int is_object)
{
  size_t i;
  enum json_type type;
  json_object_iter iter;

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
    json_object_object_foreachC(json, iter)
    {
      for (i = 0; i < indent + 1; i++) { printf("  "); }
      printf("%s: ", iter.key);
      print_json_object(iter.val, indent + 1, 1);
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
