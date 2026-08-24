#include "shell.h"

/**
 * main - entry point and main loop of the simple shell
 * @argc: argument count (unused, kept for the standard prototype)
 * @argv: argument vector; argv[0] is the program name, used in errors
 *
 * Reads a line at a time, splits it into words, and runs the command.
 * Works both interactively (shows a prompt) and non-interactively
 * (input piped in), and stops cleanly on end-of-file (Ctrl+D).
 *
 * Return: the exit status of the last command run, like sh does
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char **args;
	int line_number = 0, status = 0;

	(void)argc;
	while (1)
	{
		/* only show the prompt when a human is typing at a terminal */
		if (isatty(STDIN_FILENO))
			printf("($) ");
		nread = getline(&line, &len, stdin);
		if (nread == -1) /* EOF (Ctrl+D) or read error: leave the loop */
		{
			if (isatty(STDIN_FILENO))
				printf("\n");
			break;
		}
		line_number++;
		args = split_line(line);
		if (args[0] != NULL) /* skip empty lines / lines of only spaces */
			status = execute(args, argv[0], line_number);
		free(args);
	}
	free(line); /* getline's buffer is reused every loop, freed once here */
	return (status);
}
