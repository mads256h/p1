#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

#include "command.h"

int main(void)
{
  curl_global_init(CURL_GLOBAL_DEFAULT);

  command_loop();

  curl_global_cleanup();


  return EXIT_SUCCESS;
}
