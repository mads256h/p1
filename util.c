#include <stdio.h>
#include <string.h>
#include <time.h>

#include "util.h"


#define SECS_IN_DAY (60 * 60 * 24)


struct tm date_today(void)
{
  const time_t t = time(NULL);


  struct tm date = *gmtime(&t);

  return date;
}

struct tm date_tomorrow(struct tm date)
{
  const time_t epoch = mktime(&date) + SECS_IN_DAY;

  return *gmtime(&epoch);
}


void format_date(char date_str[DATE_SIZE + 1], const struct tm date)
{
  strftime(date_str, DATE_SIZE + 1, "%d-%m-%Y", &date);
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
