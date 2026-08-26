#include "main.h"

/**
 * _getenv - look up an environment variable by name
 * @name: variable name to search for, e.g. "PATH"
 *
 * Return: pointer to the value part inside environ, or NULL if missing
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
 * handle_builtins - check and execute internal shell commands
 * @args: parsed command arguments
 * @line: memory allocated line to free before exit
 * @last_status: status code of last command
 *
 * Return: 1 if builtin executed, 0 otherwise
 */
int handle_builtins(char **args, char *line, int last_status)
{
	int i;

	if (strcmp(args[0], "exit") == 0)
	{
		free(line);
		exit(last_status);
	}

	if (strcmp(args[0], "env") == 0)
	{
		for (i = 0; environ && environ[i]; i++)
		{
			write(STDOUT_FILENO, environ[i], strlen(environ[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		return (1);
	}

	return (0);
}
