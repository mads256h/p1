#include <stdint.h>


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
