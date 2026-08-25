#include "main.h"

/**
 * main - Simple Shell with argument handling using strtok
 * as Unused argument count
 * av Argument vector for error output
 * Return: Always 0 on success.
 */
int main(int ac, char **av)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_bytes;
	pid_t child_pid;
	int status, i;
	char *args[64];
	char *token;
	(void)ac; 
	/** all virables we need in the code */
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
	/** loop helps to see if the =order interactive or no */
	i = 0;
		token = strtok(line, " \t\r\n\a");
		while (token != NULL && i < 63)
		{
			args[i] = token;
			token = strtok(NULL, " \t\r\n\a");
			i++;
		}
		args[i] = NULL;
		/** token size input line into args array */
		if (args[0] == NULL)
			continue;

		child_pid = fork();
		if (child_pid == -1)
		{
			perror(av[0]);
			free(line);
			exit(1);
		}

		 /** here skip the wmpty input */
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

