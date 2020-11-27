#ifndef COMMAND_H
#define COMMAND_H


#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Handles help argument */
#define HANDLE_HELP(usage)                           \
  do {                                               \
    if (argc == 2 && strcmp(argv[1], "help") == 0) { \
      goto help;                                     \
    help:                                            \
      printf("USAGE: %s %s\n", argv[0], usage);      \
      fflush(stdout);                                \
      return EXIT_SUCCESS;                           \
    }                                                \
  } while (0)


/* Holds data used between commands */
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

/* Command function signature */
typedef int(
  command_func)(const size_t, const char *const[], struct command_data *const);

/* A command entry in the commands array */
struct command_entry
{
  const char *const name;
  command_func *const command;
};


/* Command function prototypes */
command_func command_help;
command_func command_quit;
command_func command_settings;
command_func command_download;
command_func command_cheapest;
command_func command_save;

/* Holds all registered commands */
static const struct command_entry commands[] = { { "help", command_help },
  { "quit", command_quit },
  { "settings", command_settings },
  { "download", command_download },
  { "cheapest", command_cheapest },
  { "save", command_save } };


/* Keeps asking the user for input and running the command until "quit" */
int command_loop(void);

#endif
