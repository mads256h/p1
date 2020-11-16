#include "math.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


size_t *find_cheapest_hours(size_t start,
  size_t end,
  double charge,
  double prices[],
  size_t *size)
{
  size_t i, j, cheapest_index, k;
  size_t *used_prices;
  int found;
  size_t hours_to_charge = (size_t)ceil(charge_to_hours(charge));
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


double charge_to_hours(double charge)
{
  double capacity_kWh = 75.0;
  double kWh_pr_hour = 7.00;

  assert(charge >= 0.0 && charge <= 1.0);

  return (capacity_kWh * (1.0 - charge)) / kWh_pr_hour;
}


double average(double numbers[], size_t size)
{
  size_t i;
  double sum = 0;

  assert(numbers);
  assert(size > 0);

  /* Summate all numbers */
  for (i = 0; i < size; i++) { sum += numbers[i]; }
  return sum / (double)size;
}
