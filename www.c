#include <assert.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "util.h"
#include "www.h"


static size_t
  write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp);

char *format_url(const char *const url, const struct tm date)
{
  const size_t url_len = strlen(url);
  char date_str[DATE_SIZE + 1];
  char *formatted_url;

  format_date(date_str, date);

  formatted_url = malloc(url_len + strlen(date_str) + 1);

  memcpy(formatted_url, url, url_len + 1);

  strcat(formatted_url, date_str);


  return formatted_url;
}

char *download_url(const char *const url)
{
  struct memory_struct mem;
  CURL *curl;

  mem.memory = 0;
  mem.size = 0;


  curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_memory_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&mem);

  curl_easy_perform(curl);

  curl_easy_cleanup(curl);


  return mem.memory;
}

static size_t
  write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
  const size_t realsize = size * nmemb;
  struct memory_struct *const mem = (struct memory_struct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  assert(ptr);

  mem->memory = ptr;

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;

  mem->memory[mem->size] = 0;

  return realsize;
}
