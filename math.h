#include <stddef.h>
#include <stdint.h>
#include <time.h>

#include "util.h"

/*Finds the cheapest hours between start and end required to charge the electric
 * car*/
size_t *find_cheapest_hours(size_t start,
  size_t end,
  double charge,
  double prices[],
  size_t *size);

/*Converts from charge to how long time to fully charge the battery in hours*/
double charge_to_hours(double charge);

/*Finds the average from numberarray*/
double average(double numbers[], size_t size);

void print_prices(const struct tm date_today,
  const struct tm date_tomorrow,
  const double prices[HOURS_USED]);


void print_cheapest_prices(const struct tm date_today,
  const struct tm date_tomorrow,
  const double prices[HOURS_USED],
  const size_t cheapest_hours[],
  const size_t cheapest_hours_length);

void print_date_hours(const struct tm date, const int hour);
