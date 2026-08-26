#ifndef MAIN_H
#define MAIN_H

/* Standard C library headers for I/O and memory management */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* System headers for processes and POSIX calls */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Global environment variable prototype for execve */
extern char **environ;
char *_getenv(const char *name);
char *get_cmd_path(char *cmd);
int run_command(char **args, char **av, int line_number);
void tokenize_line(char *line, char **args);
int handle_builtins(char **args, char *line, int last_status);
#endif /* MAIN_H */
