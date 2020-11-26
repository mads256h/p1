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
  double charge;
  double capacity;
  double rate;

  HANDLE_HELP("put number OR get");

  if (argc < 2 || argc > 4) { goto help; }

  if (argc == 4 && strcmp(argv[1], "put") == 0) {
    if (strcmp(argv[2], "charge") == 0) {
      charge = atof(argv[3]);

      if (charge > 1.0 || charge < 0.0) {
        printf("charge has to be between 0.0 and 1.0\n");
        return EXIT_SUCCESS;
      }
      data->charge = charge;
    } else if (strcmp(argv[2], "capacity") == 0) {
      capacity = atof(argv[3]);

      if (capacity <= 0.0) {
        printf("this is not an electric car\n");
        return EXIT_SUCCESS;
      }

      data->capacity = capacity;
    } else if (strcmp(argv[2], "rate") == 0) {
      rate = atof(argv[3]);

      if (rate <= 0.0) {
        printf("rate has to be positive\n");
        return EXIT_SUCCESS;
      }

      data->rate = rate;
    } else if (strcmp(argv[2], "region") == 0) {
      if (strcmp(argv[3], "dk1") == 0) {
        data->region = 1;
      } else if (strcmp(argv[3], "dk2") == 0) {
        data->region = 2;
      } else {
        printf("Invalid region\n");
      }
    }
  }

  if (argc == 3 && strcmp(argv[1], "get") == 0) {
    if (strcmp(argv[2], "charge") == 0) {
      if (data->charge == -1.0) {
        printf("charge has no value\n");
        return EXIT_SUCCESS;
      }

      printf("%f\n", data->charge);
    } else if (strcmp(argv[2], "capacity") == 0) {
      if (data->capacity == -1.0) {
        printf("capacity has no value\n");
        return EXIT_SUCCESS;
      }

      printf("%f\n", data->capacity);
    } else if (strcmp(argv[2], "rate") == 0) {
      if (data->rate == -1.0) {
        printf("rate has no value\n");
        return EXIT_SUCCESS;
      }

      printf("%f\n", data->rate);
    } else if (strcmp(argv[2], "region") == 0) {
      printf("%s\n",
        data->region == 1   ? "dk1"
        : data->region == 2 ? "dk2"
                            : "none");
    }
  }

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
  double *region_prices;
  size_t *cheapest;
  size_t length;
  size_t start, end;
  struct tm date1 = date_today(), date2;

  HANDLE_HELP("start end OR start end date1 date2");

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
  HANDLE_HELP("none");

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

struct command_data load_config(void)
{
  FILE *file;
  struct command_data data;
  char entry[50];

  file = fopen("config.cfg", "r");

  if (!file) {
    data.charge = -1.0;
    data.capacity = -1.0;
    data.rate = -1.0;
    data.region = 0;
    return data;
  }

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

int command_loop(void)
{
  int exit;
  char *line;
  struct split_string_data split;
  struct command_data data = load_config();

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
