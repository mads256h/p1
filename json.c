#include "json.h"
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>


char *read_file(const char *filename)
{
  int c;
  size_t i = 0, cur_size = BUFFER_SIZE;
  char *buffer;
  FILE *file;

  buffer = malloc(BUFFER_SIZE);
  assert(buffer);

  file = fopen(filename, "rb");
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


struct price_data extract_price_data(json_object *jso_today,
  json_object *jso_tomorrow)
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


json_object *get_jso_from_format(json_object *jso, const char *format, ...)
{
  va_list ap;
  size_t i, str_len;
  json_object *cur_jso;

  assert(jso);
  assert(format);

  cur_jso = jso;

  str_len = strlen(format);

  va_start(ap, format);

  for (i = 0; i < str_len; i++) {
    if (format[i] == 'i') {
      cur_jso = get_jso_from_array_index(cur_jso, va_arg(ap, size_t));
    } else if (format[i] == 'k') {
      cur_jso = get_jso_from_keys(cur_jso, va_arg(ap, const char *));
    } else {
      assert(0);
    }

    assert(cur_jso);
  }

  va_end(ap);


  return cur_jso;
}


double string_to_double(const char *string)
{
  char *dup;
  size_t str_len, i;
  double value;

  assert(string);

  dup = strdup(string);
  assert(dup);

  str_len = strlen(dup);

  for (i = 0; i < str_len; i++) {
    if (dup[i] == ',') { dup[i] = '.'; }
  }

  value = strtod(dup, 0);

  free(dup);

  return value;
}


json_object *get_jso_from_array_index(json_object *jso, size_t index)
{
  size_t length;
  assert(jso);

  assert(json_object_get_type(jso) == json_type_array);
  length = json_object_array_length(jso);

  printf("index: %d\n", (int)index);
  printf("length: %d\n", (int)length);
  fflush(stdout);
  
  assert(index < length);

  return json_object_array_get_idx(jso, index);
}


json_object *get_jso_from_keys(json_object *jso, const char *keys)
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


struct split_string_data split_string(const char *string, const char split_char)
{
  char *dup;
  size_t str_len, char_count = 0, i, j = 1;
  struct split_string_data ret;

  assert(string);

  dup = strdup(string);

  assert(dup);

  str_len = strlen(dup);

  /* Count the amount of 'split_char's in string */
  for (i = 0; i < str_len; i++) {
    if (dup[i] == split_char) { char_count++; }
  }

  ret.size = char_count + 1;
  ret.strings = malloc(ret.size * sizeof(char *));

  assert(ret.strings);

  ret.strings[0] = dup;

  for (i = 0; i < str_len; i++) {
    if (dup[i] == split_char) {
      dup[i] = 0;
      ret.strings[j++] = dup + i + 1;
    }
  }

  assert(j == ret.size);

  return ret;
}

void free_split_string(struct split_string_data data)
{
  assert(data.strings);
  assert(data.strings[0]);

  free(data.strings[0]);
  free(data.strings);
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
