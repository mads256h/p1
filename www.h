
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


struct memory_struct
{
  char *memory;
  size_t size;
};

char *format_url(const char *const url, const struct tm date);

char *download_url(const char *const url);
