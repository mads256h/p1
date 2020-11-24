#include "command.h"
#include "json.h"
#include "util.h"
#include "math.h"
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
  printf("hello world\n");
  fflush(stdout);
  return EXIT_SUCCESS;
}

int command_echo(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  size_t i;
  for (i = 0; i < argc; i++) { printf("%s ", argv[i]); }
  printf("\n");
  fflush(stdout);
  return EXIT_SUCCESS;
}


int command_settings(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  if (argc < 2) { return EXIT_FAILURE; }
  if (argc == 3 && strcmp(argv[1], "put") == 0) { data->test = atoi(argv[2]); }
  if (strcmp(argv[1], "get") == 0) { printf("%d\n", data->test); }
  fflush(stdout);
  return EXIT_SUCCESS;
}

int command_download(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  char *url;
  char filename[DATE_FILE_SIZE+1];
  struct tm date = date_today();
  if (argc == 2) { date = string_to_date(argv[1]); }
  url = format_url(url_part, date);
  format_date_file(filename, date);
  download_url(url, filename);
}

int command_cheapest(size_t argc,
  const char *const argv[],
  struct command_data *data)
{
  char filename[DATE_FILE_SIZE + 1];
  char *file_content;
  json_object *jso;
  struct price_data prices;
  size_t *cheapest;
  size_t length;
  int start, end;
  struct tm date = date_today();
  if (argc < 3) { return EXIT_FAILURE; }
  start = atoi(argv[1]);
  end = atoi(argv[2]);

  if (argc == 4) { date = string_to_date(argv[3]); }
  format_date_file(filename, date);
  file_content = read_file(filename);
  jso = json_tokener_parse(file_content);
  free(file_content);
  prices = extract_price_data(jso, jso);
  cheapest = find_cheapest_hours(start, end, 0, prices.dk1, &length);


  print_cheapest_prices(date, date, prices.dk1, cheapest, length);

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
  printf("%s\n", argv[0]);
  for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
    if (strcmp(argv[0], commands[i].name) == 0) {

      (commands[i].command)(argc, argv, data);
    }
  }
  return 0;
}
int command_loop(void)
{
  int exit;
  char *line;
  struct split_string_data split;
  struct command_data data;
  do {
    line = readline();
    if (!line) { continue; }
    printf("%s\n", line);
    fflush(stdout);
    split = split_string(line, ' ');
    exit = handle_command(split.size, split.strings, &data);
    free_split_string(split);
    free(line);
  } while (exit == 0);
}
