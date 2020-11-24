#include <inttypes.h>
#include <stddef.h>

struct command_data
{
  int test;
};

typedef int(command_func)(size_t, const char *const[], struct command_data *);

struct command_entry
{
  const char *name;
  command_func *command;
};

command_func command_help;
command_func command_echo;
command_func command_settings;

static const struct command_entry commands[] = { { "help", command_help },
  { "echo", command_echo },
  { "settings", command_settings } };

char *readline(void);

int handle_command(size_t argc,
  const char *const argv[],
  struct command_data *data);

int command_loop(void);
