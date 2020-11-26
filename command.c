#include "command.h"
#include "json.h"
#include "math.h"
#include "util.h"
#include "www.h"

#include <inttypes.h>
#include <json-c/json.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int command_help(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  size_t i;

  struct split_string_data arguments;
  arguments.strings = malloc(sizeof(char *) * 2);
  arguments.size = 2;
  arguments.strings[1] = "help";

  for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
    if (strcmp(commands[i].name, "help") == 0) { continue; }

    arguments.strings[0] = commands[i].name;
    commands[i].command(arguments.size, arguments.strings, data);
  }

  free(arguments.strings);

  return EXIT_SUCCESS;
}

int command_echo(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  size_t i;

  HANDLE_HELP("args");

  for (i = 1; i < argc; i++) { printf("%s ", argv[i]); }

  printf("\n");
  return EXIT_SUCCESS;
}

int command_quit(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  (void)data;

  HANDLE_HELP("");

  return EXIT_FAILURE;
}


int command_settings(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  HANDLE_HELP("put number OR get");

  if (argc < 2) { goto help; }

  if (argc == 3 && strcmp(argv[1], "put") == 0) { data->test = atoi(argv[2]); }
  if (strcmp(argv[1], "get") == 0) { printf("%d\n", data->test); }

  return EXIT_SUCCESS;
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

  HANDLE_HELP("none OR date");

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
  size_t *cheapest;
  size_t length;
  size_t start, end;
  struct tm date1 = date_today(), date2;

  HANDLE_HELP("start end OR start end date1 date2");

  if (argc < 3) { goto help; }

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
    perror("could not open file");
    return EXIT_SUCCESS;
  }
  jso_today = json_tokener_parse(file_content);
  free(file_content);

  format_date_file(filename, date2);
  if (!read_file(filename, &file_content)) {
    json_object_put(jso_today);
    perror("could not open file");
    return EXIT_SUCCESS;
  }

  jso_tomorrow = json_tokener_parse(file_content);
  free(file_content);

  prices = extract_price_data(jso_today, jso_tomorrow);
  json_object_put(jso_today);
  json_object_put(jso_tomorrow);

  cheapest = find_cheapest_hours(start, end, 0, prices.dk1, &length);
  print_cheapest_prices(date1, date2, prices.dk1, cheapest, length);
  free(cheapest);

  return EXIT_SUCCESS;
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
  int ret = -1;
  for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
    if (strcmp(argv[0], commands[i].name) == 0) {
      ret = commands[i].command(argc, argv, data);
    }
  }

  if (ret == -1) {
    printf("Command not found\n");
    return 0;
  }

  return ret;
}

int command_loop(void)
{
  int exit;
  char *line;
  struct split_string_data split;
  struct command_data data;
  do {
    fflush(stdout);
    line = readline();
    if (!line) { continue; }

    split = split_string(line, ' ');
    exit = handle_command(split.size, split.strings, &data);
    free_split_string(split);
    free(line);
  } while (exit == 0);

  return exit;
}
