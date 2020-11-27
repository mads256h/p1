#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "math.h"


#define HOURS_IN_DAY 24


/*Converts from charge to how long time to fully charge the battery in hours*/
double charge_to_hours(const double charge,
  const double capacity,
  const double charge_rate);

void print_prices_day(const struct tm date,
  const size_t start,
  const double prices[HOURS_USED]);

void print_date_hours(const struct tm date, const int hour);


size_t *find_cheapest_hours(const size_t start,
  const size_t end,
  const double charge,
  const double capacity,
  const double charge_rate,
  const double prices[],
  size_t *const size)
{
  size_t i, j, cheapest_index, k;
  size_t *used_prices;
  int found;
  size_t hours_to_charge =
    (size_t)ceil(charge_to_hours(charge, capacity, charge_rate));
  used_prices = malloc(hours_to_charge * sizeof(size_t));

  assert(start < end);
  assert(prices);
  assert(size);

  assert(used_prices);

  /* Loop through each hour it takes to charge the car.
   * And find the cheapest hours in the time range denoted by
   * start and end*/
  for (i = 0; i < hours_to_charge; i++) {
    cheapest_index = SIZE_MAX;
    for (j = start; j < end; j++) {
      found = 0;
      for (k = 0; k < i; k++) {
        if (used_prices[k] == j) {
          found = 1;
          break;
        }
      }
      if (!found
          && (cheapest_index == SIZE_MAX
              || (prices[j] < prices[cheapest_index]))) {
        cheapest_index = j;
      }
    }
    used_prices[i] = cheapest_index;
  }
  *size = hours_to_charge;
  return used_prices;
}

double average(const double numbers[], const size_t size)
{
  size_t i;
  double sum = 0;

  assert(numbers);
  assert(size > 0);

  /* Summate all numbers */
  for (i = 0; i < size; i++) { sum += numbers[i]; }
  return sum / (double)size;
}


void print_prices(const struct tm date_today,
  const struct tm date_tomorrow,
  const double prices[HOURS_USED])
{
  /* Today */
  print_prices_day(date_today, 0, prices);

  /* Tomorrow */
  print_prices_day(date_tomorrow, HOURS_USED / 2, prices);
}

void print_cheapest_prices(const struct tm date_today,
  const struct tm date_tomorrow,
  const double prices[HOURS_USED],
  const size_t cheapest_hours[],
  const size_t cheapest_hours_length)
{
  size_t i, hour;
  double prices_average, cheapest_average;
  double *selected_price;

  for (i = 0; i < cheapest_hours_length; i++) {
    hour = cheapest_hours[i];
    if (hour < HOURS_USED / 2) {
      print_date_hours(date_today, hour);
    } else {
      print_date_hours(date_tomorrow, hour - (HOURS_USED / 2));
    }


    printf(": %.2fDKK\n", prices[cheapest_hours[i]]);
  }
  selected_price = malloc(cheapest_hours_length * sizeof(double));
  for (i = 0; i < cheapest_hours_length; i++) {
    selected_price[i] = prices[cheapest_hours[i]];
  }
  prices_average = average(prices, HOURS_USED);
  cheapest_average = average(selected_price, cheapest_hours_length);
  free(selected_price);
  printf("Saved: %.2f%%\n", (1.0 - cheapest_average / prices_average) * 100.0);
}

double charge_to_hours(const double charge,
  const double capacity,
  const double charge_rate)
{
  assert(charge >= 0.0 && charge <= 1.0);
  assert(capacity > 0.0);
  assert(charge_rate > 0.0);

  return (capacity * (1.0 - charge)) / charge_rate;
}


void print_prices_day(const struct tm date,
  const size_t start,
  const double prices[HOURS_USED])
{
  size_t i;
  for (i = start; i < start + HOURS_IN_DAY; i++) {
    print_date_hours(date, i - start);

    printf(": %.2fDKK\n", prices[i]);
  }
}

void print_date_hours(const struct tm date, const int hour)
{
  char date_str[DATE_SIZE + 1];

  format_date(date_str, date);


  printf("%s T %02d-%02d", date_str, hour, hour + 1);
}
