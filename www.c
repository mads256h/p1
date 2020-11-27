#include <assert.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "util.h"
#include "www.h"


char *format_url(const char *const url, const struct tm date)
{
  const size_t url_len = strlen(url);
  char date_str[DATE_SIZE + 1];
  char *formatted_url;

  assert(url);

  format_date(date_str, date);

  formatted_url = malloc(url_len + strlen(date_str) + 1);

  assert(formatted_url);

  memcpy(formatted_url, url, url_len + 1);

  strcat(formatted_url, date_str);


  return formatted_url;
}

void download_url(const char *const url, const char *const filename)
{
  FILE *const file = fopen(filename, "w");
  CURL *const curl = curl_easy_init();

  assert(url);
  assert(file);
  assert(curl);


  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

  curl_easy_perform(curl);

  curl_easy_cleanup(curl);

  fclose(file);
}
