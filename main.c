#include "main.h"

/**
 * main - Entry point for simple shell 0.1 handling spaces
 * @ac: Unused argument count
 * @av: Argument vector for error reporting
 *
 * Return: Always 0 on success.
 */
int main(int ac, char **av)
{
	char *line = NULL, *start;
	size_t len = 0;
	ssize_t read_bytes;
	pid_t child_pid;
	int status;
	char *args[2];
	(void)ac;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		read_bytes = getline(&line, &len, stdin);
		if (read_bytes == -1)
		{
			free(line);
			break;
		}

		/* 1. Remove newline at the end */
		if (line[read_bytes - 1] == '\n')
			line[read_bytes - 1] = '\0';

		/* 2. Skip leading spaces */
		start = line;
		while (*start == ' ' || *start == '\t')
			start++;

		/* 3. Skip empty line or lines with spaces only */
		if (*start == '\0')
			continue;

		/* 4. Remove trailing spaces */
		len = strlen(start);
		while (len > 0 && (start[len - 1] == ' ' || start[len - 1] == '\t'))
		{
			start[len - 1] = '\0';
			len--;
		}

		/* Prepare clean command for execve */
		args[0] = start;
		args[1] = NULL;

		child_pid = fork();
		if (child_pid == -1)
		{
			perror(av[0]);
			free(line);
			exit(1);
		}

		if (child_pid == 0)
		{
			if (execve(args[0], args, environ) == -1)
			{
				perror(av[0]);
				free(line);
				exit(1);
			}
		}
		else
		{
			wait(&status);
		}
	}
	return (0);
}
