#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* environ is provided by the C library: the current process environment */
extern char **environ;

char **split_line(char *line);
int execute(char **args, char *prog_name, int line_number);

#endif /* SHELL_H */
