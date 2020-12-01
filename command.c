#include "command.h"
#include "json.h"
#include "math.h"
#include "util.h"
#include "www.h"

#include <assert.h>
#include <inttypes.h>
#include <json-c/json.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Converts a macro function parameter to string */
#define TO_STRING(s) TO_STRING_IMPL(s)
#define TO_STRING_IMPL(s) #s

/* Sets a key in settings */
#define SETTINGS_SET_DOUBLE_HELPER(key, condition, errortext) \
  if (strcmp(argv[2], TO_STRING(key)) == 0) {                 \
    value = atof(argv[3]);                                    \
    if (condition) {                                          \
      printf(errortext "\n");                                 \
      return EXIT_SUCCESS;                                    \
    }                                                         \
    data->key = value;                                        \
    return EXIT_SUCCESS;                                      \
  }

/* Gets a key in settings */
#define SETTINGS_GET_DOUBLE_HELPER(key)       \
  if (strcmp(argv[2], TO_STRING(key)) == 0) { \
    if (data->key == -1.0) {                  \
      printf(TO_STRING(key) " is not set\n"); \
      return EXIT_SUCCESS;                    \
    }                                         \
    printf("%f\n", data->key);                \
    return EXIT_SUCCESS;                      \
  }

/* Loads the users saved config */
struct command_data load_config(void);

/* Reads in a line from the terminal */
char *readline(void);

/* Runs the command specified */
int handle_command(size_t argc,
  const char *const argv[],
  struct command_data *data);


int command_loop(void)
{
  int exit;
  char *line;
  struct split_string_data split;
  struct command_data data = load_config();

  do {
    printf(">");
    fflush(stdout);
    line = readline();
    if (!line) { continue; }

    split = split_string(line, ' ');
    exit =
      handle_command(split.size, (const char *const *)split.strings, &data);
    free_split_string(split);
    free(line);
  } while (exit == 0);

  return exit;
}

struct command_data load_config(void)
{
  FILE *const file = fopen("config.cfg", "r");
  struct command_data data;
  char entry[50];


  /* If "config.cfg" does not exist use default values */
  if (!file) {
    data.charge = -1.0;
    data.capacity = -1.0;
    data.rate = -1.0;
    data.region = 0;
    return data;
  }

  /* While there is a line to read */
  while (fscanf(file, "%[^ ]", entry) == 1) {
    if (strcmp(entry, "charge") == 0) {
      fscanf(file, "%lf\n", &data.charge);
    } else if (strcmp(entry, "capacity") == 0) {
      fscanf(file, "%lf\n", &data.capacity);
    } else if (strcmp(entry, "rate") == 0) {
      fscanf(file, "%lf\n", &data.rate);
    } else if (strcmp(entry, "region") == 0) {
      fscanf(file, "%d\n", &data.region);
    }
  }

  fclose(file);


  return data;
}

char *readline(void)
{
  size_t i = 0;
  int c = EOF;
  char *string = 0;

  while (c = fgetc(stdin), c != EOF && c != '\n') {
    string = realloc(string, i + 2);

    string[i] = (char)c;

    i++;
  }

  if (string) { string[i] = 0; }

  return string;
}

int handle_command(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  size_t i;

  /* Go through each command and find the one specified in argv[0] */
  for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
    if (strcmp(argv[0], commands[i].name) == 0) {
      return commands[i].command(argc, argv, data);
    }
  }

  printf("Command not found\n");
  return EXIT_SUCCESS;
}


int command_help(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  size_t i;
  const char *arguments[2];

  arguments[1] = "help";

  (void)argc;
  (void)argv;

  /* Go through each command and run it with the argument "help" */
  for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
    if (strcmp(commands[i].name, "help") == 0) { continue; }

    arguments[0] = commands[i].name;
    commands[i].command(2, arguments, data);
  }

  return EXIT_SUCCESS;
}


int command_quit(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  (void)data;

  HANDLE_HELP("");

  /* EXIT_FAILURE stops the command loop */
  return EXIT_FAILURE;
}


int command_settings(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  double value;

  HANDLE_HELP("set <key> <value> OR get <key>");

  if (argc < 2 || argc > 4) { goto help; }

  /* If the user types in set */
  if (argc == 4 && strcmp(argv[1], "set") == 0) {
    SETTINGS_SET_DOUBLE_HELPER(charge,
      value > 1.0 || value < 0.0,
      "charge has to be between 0.0 and 1.0");
    SETTINGS_SET_DOUBLE_HELPER(
      capacity, value <= 0.0, "this is not an electric car");
    SETTINGS_SET_DOUBLE_HELPER(rate, value <= 0.0, "rate has to be positive");

    if (strcmp(argv[2], "region") == 0) {
      if (strcmp(argv[3], "dk1") == 0) {
        data->region = 1;
      } else if (strcmp(argv[3], "dk2") == 0) {
        data->region = 2;
      } else {
        printf("Invalid region\n");
      }
      return EXIT_SUCCESS;
    }
  }

  /* If the user types in get */
  if (argc == 3 && strcmp(argv[1], "get") == 0) {
    SETTINGS_GET_DOUBLE_HELPER(charge);
    SETTINGS_GET_DOUBLE_HELPER(capacity);
    SETTINGS_GET_DOUBLE_HELPER(rate);

    if (strcmp(argv[2], "region") == 0) {
      printf("%s\n",
        data->region == 1   ? "dk1"
        : data->region == 2 ? "dk2"
                            : "none");
      return EXIT_SUCCESS;
    }
  }

  goto help;
}

int command_download(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  char *url;
  char filename[DATE_FILE_SIZE + 1] = { 0 };
  struct tm date = date_today();
  const struct tm today = date_today();
  const struct tm tomorrow = date_tomorrow(today);

  HANDLE_HELP("[date]");
  (void)data;

  if (argc == 2) { date = string_to_date(argv[1]); }

  if (!(date_compare(date, today) <= 0
        || (date_compare(date, tomorrow) == 0 && today.tm_hour >= 12))) {
    printf("Cannot download this date\n");
    return EXIT_SUCCESS;
  }

  url = format_url(url_part, date);
  format_date_file(filename, date);

  download_url(url, filename);
  free(url);

  printf("Downloaded \"%s\"\n", filename);

  return EXIT_SUCCESS;
}

int command_cheapest(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  char filename[DATE_FILE_SIZE + 1] = { 0 };
  char *file_content;
  json_object *jso_today, *jso_tomorrow;
  struct price_data prices;
  double *region_prices;
  size_t *cheapest;
  size_t length;
  size_t start, end;
  struct tm date1 = date_today(), date2;

  HANDLE_HELP("<start> <end> [date]");

  if (argc < 3) { goto help; }

  if (data->charge == -1.0) {
    printf("No charge is set\n");
    return EXIT_SUCCESS;
  }
  if (data->capacity == -1.0) {
    printf("No capacity is set\n");
    return EXIT_SUCCESS;
  }
  if (data->rate == -1.0) {
    printf("No charge rate is set\n");
    return EXIT_SUCCESS;
  }
  if (data->region == 0) {
    printf("No region is set\n");
    return EXIT_SUCCESS;
  }

  start = (size_t)atoi(argv[1]);
  end = (size_t)atoi(argv[2]);

  if (argc == 4) {
    date1 = string_to_date(argv[3]);
  } else if (argc > 4) {
    goto help;
  }

  date2 = date_tomorrow(date1);

  format_date_file(filename, date1);
  if (!read_file(filename, &file_content)) {
    perror("data not downloaded");
    return EXIT_SUCCESS;
  }
  jso_today = json_tokener_parse(file_content);
  free(file_content);

  format_date_file(filename, date2);
  if (!read_file(filename, &file_content)) {
    json_object_put(jso_today);
    perror("data not downloaded");
    return EXIT_SUCCESS;
  }

  jso_tomorrow = json_tokener_parse(file_content);
  free(file_content);

  prices = extract_price_data(jso_today, jso_tomorrow);
  json_object_put(jso_today);
  json_object_put(jso_tomorrow);

  if (data->region == 1) {
    region_prices = prices.dk1;
  } else {
    region_prices = prices.dk2;
  }

  cheapest = find_cheapest_hours(start,
    end,
    data->charge,
    data->capacity,
    data->rate,
    region_prices,
    &length);
  print_cheapest_prices(date1, date2, region_prices, cheapest, length);

  free(cheapest);

  return EXIT_SUCCESS;
}

int command_save(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  FILE *file;
  HANDLE_HELP("");

  if (argc > 1) { goto help; }


  file = fopen("config.cfg", "w");

  assert(file);

  fprintf(file, "charge %f\n", data->charge);
  fprintf(file, "capacity %f\n", data->capacity);
  fprintf(file, "rate %f\n", data->rate);
  fprintf(file, "region %d\n", data->region);


  fclose(file);


  return EXIT_SUCCESS;
}
