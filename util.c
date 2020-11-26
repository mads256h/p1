#include <stdio.h>
#include <string.h>
#include <time.h>

#include "util.h"

#define SECS_IN_DAY (60 * 60 * 24)

struct tm date_today(void)
{
  time_t t = time(NULL);
  return *gmtime(&t);
}

struct tm date_tomorrow(void)
{
  time_t t = time(NULL) + SECS_IN_DAY;
  return *gmtime(&t);
}

void format_date(char date_str[DATE_SIZE + 1], const struct tm date)
{
  snprintf(date_str,
    DATE_SIZE + 1,
    "%d-%d-%d",
    date.tm_mday,
    date.tm_mon + 1,
    date.tm_year + 1900);
}

void format_date_file(char date_file_str[DATE_FILE_SIZE + 1],
  const struct tm date)
{
  format_date(date_file_str, date);
  strcat(date_file_str, ".json");
}

struct tm string_to_date(const char *const date_string)
{
  struct tm date;
  sscanf(date_string, "%d-%d-%d", &date.tm_mday, &date.tm_mon, &date.tm_year);
  date.tm_mon--;
  date.tm_year -= 1900;
  return date;
}