#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "www.h"


static size_t
  write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp);


char *download_url(const char *const url)
{
  struct memory_struct mem;
  CURL *curl;

  mem.memory = malloc(1);
  mem.size = 0;



  curl = curl_easy_init();
  
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_memory_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&mem);

  curl_easy_perform(curl);

  curl_easy_cleanup(curl);


  return mem.memory;
}

static size_t
  write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct memory_struct *mem = (struct memory_struct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}
