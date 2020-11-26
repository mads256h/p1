#include <stddef.h>
#include <stdint.h>
#include <time.h>

#include "util.h"

/*Finds the cheapest hours between start and end required to charge the electric
 * car*/
size_t *find_cheapest_hours(const size_t start,
  const size_t end,
  const double charge,
  const double capacity,
  const double charge_rate,
  const double prices[],
  size_t * const size);

/*Converts from charge to how long time to fully charge the battery in hours*/
double charge_to_hours(const double charge, const double capacity, const double charge_rate);

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
