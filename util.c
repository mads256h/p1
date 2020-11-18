#include <stdio.h>
#include <time.h>

#include "util.h"

#define SECS_IN_DAY (60 * 60 * 24)

struct tm *today(void)
{
  time_t t = time(NULL);
  return gmtime(&t);
}

struct tm *tomorrow(void)
{
  time_t t = time(NULL) + SECS_IN_DAY;
  return gmtime(&t);
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

struct command_data
{
  int use_dk1;
};