#if NDEBUG
#undef NDEBUG
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"


struct price_data get_expected(void)
{
  struct price_data expected;
  expected.dk1[0] = 11.04;
  expected.dk1[1] = 10.26;
  expected.dk1[2] = 9.63;
  expected.dk1[3] = 9.99;
  expected.dk1[4] = 11.23;
  expected.dk1[5] = 13.00;
  expected.dk1[6] = 14.06;
  expected.dk1[7] = 16.05;
  expected.dk1[8] = 18.24;
  expected.dk1[9] = 15.98;
  expected.dk1[10] = 14.69;
  expected.dk1[11] = 14.25;
  expected.dk1[12] = 14.06;
  expected.dk1[13] = 14.02;
  expected.dk1[14] = 14.07;
  expected.dk1[15] = 14.09;
  expected.dk1[16] = 14.42;
  expected.dk1[17] = 15.02;
  expected.dk1[18] = 14.59;
  expected.dk1[19] = 14.03;
  expected.dk1[20] = 12.85;
  expected.dk1[21] = 11.93;
  expected.dk1[22] = 11.15;
  expected.dk1[23] = 7.61;

  expected.dk2[0] = 11.04;
  expected.dk2[1] = 10.26;
  expected.dk2[2] = 9.63;
  expected.dk2[3] = 9.99;
  expected.dk2[4] = 11.23;
  expected.dk2[5] = 13.00;
  expected.dk2[6] = 14.06;
  expected.dk2[7] = 44.34;
  expected.dk2[8] = 43.10;
  expected.dk2[9] = 21.07;
  expected.dk2[10] = 14.69;
  expected.dk2[11] = 14.25;
  expected.dk2[12] = 14.06;
  expected.dk2[13] = 14.02;
  expected.dk2[14] = 14.07;
  expected.dk2[15] = 14.09;
  expected.dk2[16] = 14.42;
  expected.dk2[17] = 43.09;
  expected.dk2[18] = 23.70;
  expected.dk2[19] = 14.03;
  expected.dk2[20] = 12.85;
  expected.dk2[21] = 11.93;
  expected.dk2[22] = 11.15;
  expected.dk2[23] = 7.61;


  return expected;
}


int main(void)
{
  char *json_content = read_file("test_extract_price_data.json");

  assert(json_content);

  json_object *jso = json_tokener_parse(json_content);

  free(json_content);

  assert(jso);

  struct price_data actual = extract_price_data(jso);

  struct price_data expected = get_expected();

  assert(memcmp(&expected, &actual, sizeof(struct price_data)) == 0);

  json_object_put(jso);

  return EXIT_SUCCESS;
}
