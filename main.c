#include "main.h"

/**
 * _getenv - look up an environment variable by name
 * @name: variable name to search for, e.g. "PATH"
 *
 * getenv() is not on the list of allowed functions, so this walks
 * environ by hand looking for "name=value".
 *
 * Return: pointer to the value part inside environ, or NULL if the
 * variable isn't set
 */
char *_getenv(const char *name)
{
	int i;
	size_t len;

	len = strlen(name);
	for (i = 0; environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (environ[i] + len + 1);
	}
	return (NULL);
}

/**
 * get_cmd_path - find the full path to run a command
 * @cmd: the command as typed by the user
 *
 * If cmd already contains a '/', it's treated as a path already (like
 * /bin/ls or ./hsh) and just checked directly. Otherwise, every
 * directory in PATH is tried until one has an executable with that
 * name.
 *
 * Return: a malloc'd string with the full path, or NULL if the
 * command can't be found anywhere
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
 * @args: NULL-terminated argument vector, args[0] is the command
 * @av: the shell's own argv, av[0] is used as the program name in
 * error messages
 * @line_number: number of the input line this command came from, used
 * in the "not found" error the same way sh uses it
 *
 * If args[0] can't be resolved to a real file, we print the error and
 * return without forking at all (no point forking to run nothing).
 *
 * Return: the command's exit status (127 if it wasn't found), so the
 * shell can exit with the same status sh would; or -1 if fork()
 * itself failed, meaning the shell should give up and exit
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
 * @line: the line to split; strtok cuts it in place
 * @args: array to fill in, must be able to hold at least 64 pointers
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
 * main - Simple Shell entry point, with PATH resolution and argument
 * handling
 * @ac: argument count (unused)
 * @av: argument vector; av[0] is used as the program name in errors
 *
 * Return: the exit status of the last command run, like sh does
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

		if (strcmp(args[0], "exit") == 0)
		{
			free(line);
			exit(last_status);
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

