#ifndef WWW_H
#define WWW_H
#include <time.h>

static const char url_part[] =
  "https://www.nordpoolgroup.com/api/marketdata/page/"
  "41?currency=,DKK,DKK,DKK&endDate=";

char *format_url(const char *const url, const struct tm date);

void download_url(const char *const url, const char *const filename);

#endif
