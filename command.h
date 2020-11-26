#include <inttypes.h>
#include <stddef.h>


#define HANDLE_HELP(usage)                           \
  do {                                               \
    if (argc == 2 && strcmp(argv[1], "help") == 0) { \
    help:                                            \
      printf("USAGE: %s %s\n", argv[0], usage);      \
      fflush(stdout);                                \
      return EXIT_SUCCESS;                           \
    }                                                \
  } while (0)


struct command_data
{
  int test;

  /* capacity kWh */
  double capacity;

  /* charge percent */
  double charge;
};

typedef int(command_func)(size_t, const char *const[], struct command_data *);

struct command_entry
{
  const char *const name;
  command_func *const command;
};

command_func command_help;
command_func command_echo;
command_func command_quit;
command_func command_settings;
command_func command_download;
command_func command_cheapest;

static const struct command_entry commands[] = { { "help", command_help },
  { "echo", command_echo },
  { "quit", command_quit },
  { "settings", command_settings },
  { "download", command_download },
  { "cheapest", command_cheapest } };

char *readline(void);

int handle_command(size_t argc,
  const char *const argv[],
  struct command_data *data);

int command_loop(void);
