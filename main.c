#include "main.h"

/**
 * get_cmd_path - find the full path to run a command
 * @cmd: the command as typed by the user
 *
 * Return: malloc'd string with full path, or NULL if not found
 */
char *get_cmd_path(char *cmd)
{
	char *path_env, *path_copy, *dir, *full_path;
	size_t len;

	if (strchr(cmd, '/') != NULL)
	{
		if (access(cmd, X_OK) != 0)
			return (NULL);
		full_path = malloc(strlen(cmd) + 1);
		if (full_path != NULL)
			strcpy(full_path, cmd);
		return (full_path);
	}

	path_env = _getenv("PATH");
	if (path_env == NULL)
		return (NULL);
	path_copy = malloc(strlen(path_env) + 1);
	if (path_copy == NULL)
		return (NULL);
	strcpy(path_copy, path_env);

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		len = strlen(dir) + strlen(cmd) + 2;
		full_path = malloc(len);
		if (full_path == NULL)
			break;
		sprintf(full_path, "%s/%s", dir, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

/**
 * run_command - resolve a command through PATH, then fork and run it
 * @args: NULL-terminated argument vector
 * @av: shell argv for error messaging
 * @line_number: current line number for errors
 *
 * Return: command exit status or 127 if not found, -1 on fork failure
 */
int run_command(char **args, char **av, int line_number)
{
	char *cmd_path;
	pid_t child_pid;
	int status = 0;

	cmd_path = get_cmd_path(args[0]);
	if (cmd_path == NULL)
	{
		fprintf(stderr, "%s: %d: %s: not found\n",
			av[0], line_number, args[0]);
		return (127);
	}

	child_pid = fork();
	if (child_pid == -1)
	{
		perror(av[0]);
		free(cmd_path);
		return (-1);
	}

	if (child_pid == 0)
	{
		if (execve(cmd_path, args, environ) == -1)
		{
			perror(av[0]);
			free(cmd_path);
			exit(1);
		}
	}
	else
	{
		wait(&status);
		free(cmd_path);
	}
	return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}

/**
 * tokenize_line - split a line into a NULL-terminated array of words
 * @line: line to split
 * @args: array to fill with tokens
 */
void tokenize_line(char *line, char **args)
{
	int i = 0;
	char *token;

	token = strtok(line, " \t\r\n\a");
	while (token != NULL && i < 63)
	{
		args[i] = token;
		token = strtok(NULL, " \t\r\n\a");
		i++;
	}
	args[i] = NULL;
}

/**
 * main - Simple Shell entry point
 * @ac: argument count (unused)
 * @av: argument vector
 *
 * Return: exit status of last command run
 */
int main(int ac, char **av)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_bytes;
	int line_number = 0, last_status = 0;
	char *args[64];
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
		line_number++;
		tokenize_line(line, args);
		if (args[0] == NULL)
			continue;

		if (handle_builtins(args, line, last_status))
		{
			last_status = 0;
			continue;
		}

		last_status = run_command(args, av, line_number);
		if (last_status == -1)
		{
			free(line);
			exit(1);
		}
	}
	return (last_status);
}
