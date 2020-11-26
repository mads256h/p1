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
  /* capacity kWh */
  double capacity;

  /* charge rate */
  double rate;

  /* charge percent */
  double charge;

  /* 0 for none 1 for dk1 2 for dk2 */
  int region;
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
command_func command_save;

static const struct command_entry commands[] = { { "help", command_help },
  { "echo", command_echo },
  { "quit", command_quit },
  { "settings", command_settings },
  { "download", command_download },
  { "cheapest", command_cheapest },
  { "save", command_save } };

char *readline(void);

int handle_command(size_t argc,
  const char *const argv[],
  struct command_data *data);

int command_loop(void);
