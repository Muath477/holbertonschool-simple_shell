#include "main.h"

/**
 * main - Entry point for our simple shell 0.1
 * @ac: Argument count (we don't use it here)
 * @av: Argument vector (holds program name for perror)
 *
 * Return: Always 0 when it runs smoothly.
 */
int main(int ac, char **av)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_bytes;
	pid_t child_pid;
	int status;
	char *args[2];
	(void)ac;

	while (1)
	{
		/* Print prompt only when typing live in interactive mode */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		/* Read line from user, clean memory if EOF (Ctrl+D) is hit */
		read_bytes = getline(&line, &len, stdin);
		if (read_bytes == -1)
		{
			free(line);
			break;
		}

		/* Remove trailing \n so execve gets clean file paths */
		if (line[read_bytes - 1] == '\n')
			line[read_bytes - 1] = '\0';

		/* Skip empty lines if user just hits enter */
		if (line[0] == '\0')
			continue;

		/* Prepare single-word command array ending with NULL for execve */
		args[0] = line;
		args[1] = NULL;

		/* Fork a new process to run our command safely */
		child_pid = fork();
		if (child_pid == -1)
		{
			perror(av[0]);
			free(line);
			exit(1);
		}

		/* Inside child process: execute the given command */
		if (child_pid == 0)
		{
			if (execve(args[0], args, environ) == -1)
			{
				perror(av[0]);
				free(line);
				exit(1);
			}
		}
		/* Parent process: sit tight and wait until child finishes */
		else
		{
			wait(&status);
		}
	}
	return (0);
}
