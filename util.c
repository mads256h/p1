#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "util.h"


#define SECS_IN_DAY (60 * 60 * 24)

char *str_dup(const char *const str);

int read_file(const char *filename, /* out */ char **const content)
{
  int c;

  size_t i = 0, cur_size = 0;
  FILE *file;

  file = fopen(filename, "r");

  if (!file) { return 0; }

  *content = 0;

  /* While there is something to read */
  while ((c = fgetc(file)) != EOF) {
    if ((i + 2) > cur_size) {
      cur_size += BUFFER_SIZE;
      *content = realloc(*content, cur_size);
      assert(*content);
      assert(cur_size > i + 2);
    }
    (*content)[i] = (char)c;
    i++;
  }

  if (!*content) { return 0; }

  (*content)[i] = 0;
  fclose(file);

  assert(i < cur_size);

  return 1;
}

struct tm date_today(void)
{
  const time_t t = time(NULL);


  struct tm date = *gmtime(&t);

  return date;
}

struct tm date_tomorrow(const struct tm date)
{
  struct tm tmp_date = date;
  const time_t epoch = mktime(&tmp_date) + SECS_IN_DAY;

  return *gmtime(&epoch);
}

struct tm string_to_date(const char *const date_string)
{
  struct tm date = date_today();
  sscanf(date_string, "%d-%d-%d", &date.tm_mday, &date.tm_mon, &date.tm_year);

  date.tm_year -= 1900;
  date.tm_mon -= 1;

  return date;
}


int date_compare(const struct tm a, const struct tm b)
{
  const int year_diff = a.tm_year - b.tm_year;
  const int month_diff = a.tm_mon - b.tm_mon;
  const int day_diff = a.tm_mday - b.tm_mday;

  if (year_diff != 0) { return year_diff; }
  if (month_diff != 0) { return month_diff; }
  return day_diff;
}


double string_to_double(const char *string)
{
  char *dup;
  size_t str_len, i;
  double value;

  assert(string);

  dup = str_dup(string);
  assert(dup);

  str_len = strlen(dup);

  for (i = 0; i < str_len; i++) {
    if (dup[i] == ',') { dup[i] = '.'; }
  }

  value = strtod(dup, 0);

  free(dup);

  return value;
}

struct split_string_data split_string(const char *string, const char split_char)
{
  char *dup;
  size_t str_len, char_count = 0, i, j = 1;
  struct split_string_data ret;

  assert(string);

  dup = str_dup(string);

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

char *str_dup(const char *const str)
{
  const size_t len = strlen(str);

  /* Allocate string + null terminator */
  char *const ret = malloc(len + 1);

  assert(ret);

  /* Copy str to ret */
  strcpy(ret, str);


  return ret;
}


void format_date_file(char date_file_str[DATE_FILE_SIZE + 1],
  const struct tm date)
{
  format_date(date_file_str, date);
  strcat(date_file_str, ".json");
}

void format_date(char date_str[DATE_SIZE + 1], const struct tm date)
{
  strftime(date_str, DATE_SIZE + 1, "%d-%m-%Y", &date);
}
