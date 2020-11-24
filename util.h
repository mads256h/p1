#include <stdlib.h>
#include <time.h>


#define HOURS_USED 48

/* size of string "00-00-0000" */
#define DATE_SIZE 10
#define DATE_FILE_SIZE 15

struct tm date_today(void);
struct tm date_tomorrow(void);

void format_date(char date_str[DATE_SIZE + 1], const struct tm date);
void format_date_file(char date_file_str[DATE_FILE_SIZE + 1], const struct tm date);
struct tm string_to_date(const char * const date_string);