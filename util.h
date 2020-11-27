#ifndef UTIL_H
#define UTIL_H


#include <stddef.h>
#include <time.h>


#define HOURS_USED 48

#define BUFFER_SIZE ((size_t)512)

/* size of string "00-00-0000" */
#define DATE_SIZE 10

/* size of string "00-00-0000.json" */
#define DATE_FILE_SIZE 15


/*Holds the strings split data*/
struct split_string_data
{
  size_t size;
  char **strings;
};

/*Reads the file with the filename, returns true if successful
  Remember to call free(free the memory)*/
int read_file(const char *filename, /* out */ char **const content);

/* Returns the date today */
struct tm date_today(void);

/* Takes a date and returns the next day */
struct tm date_tomorrow(const struct tm date);

/* Converts a string to a date */
struct tm string_to_date(const char *const date_string);

/* Compares two dates */
int date_compare(const struct tm a, const struct tm b);


/*Converts string to double, changes , to .*/
double string_to_double(const char *string);

/*Splits a string by 'split_char'*/
struct split_string_data split_string(const char *string,
  const char split_char);

/*Frees the struct returned from split_string*/
void free_split_string(struct split_string_data data);


/* Formats a date to a string with ".json" appended */
void format_date_file(char date_file_str[DATE_FILE_SIZE + 1],
  const struct tm date);

/* Formats a date to a string */
void format_date(char date_str[DATE_SIZE + 1], const struct tm date);

#endif
