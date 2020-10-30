#include "math.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>


size_t *find_cheapest_hours(size_t start,
  size_t end,
  double charge,
  double prices[],
  size_t *size)
{
  size_t i, j, c, k;
  size_t *used;
  int found;
  size_t hours = (size_t)ceil(charge_to_hours(charge));
  used = malloc(hours * sizeof(size_t));

  assert(start < end);
  assert(prices);
  assert(size);

  assert(used);

  for (i = 0; i < hours; i++) {
    c = SIZE_MAX;
    for (j = start; j < end; j++) {
      found = 0;
      for (k = 0; k < i; k++) {
        if (used[k] == j) {
          found = 1;
          break;
        }
      }
      if (!found && (c == SIZE_MAX || (prices[j] < prices[c]))) { c = j; }
    }
    used[i] = c;
  }
  *size = hours;
  return used;
}

double charge_to_hours(double charge)
{
  double capacity_kWh = 42.0;
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

  for (i = 0; i < size; i++) { sum += numbers[i]; }
  return sum / (double)size;
}
