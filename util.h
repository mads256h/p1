#include <stdlib.h>
#include <time.h>


#define HOURS_USED 48

/* size of string "00-00-0000" */
#define DATE_SIZE 10


struct tm *today(void);
struct tm *tomorrow(void);

void format_date(char date_str[DATE_SIZE + 1], const struct tm date);
