#include "shell.h"

/**
 * split_line - break a command line into an array of words (tokens)
 * @line: the line read by getline; it still holds the trailing newline
 *
 * strtok cuts @line in place and hands back pointers into it, so the
 * returned array's strings live inside @line. That means the caller
 * frees the array itself, but must NOT free each string, and must keep
 * @line alive while the array is in use.
 *
 * Return: a NULL-terminated array of the words, or exits on malloc failure
 */
char **split_line(char *line)
{
	int bufsize = 64, i = 0;
	char **tokens = malloc(sizeof(char *) * bufsize);
	char *token;

	if (tokens == NULL)
	{
		perror("hsh");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, " \t\r\n");
	while (token != NULL)
	{
		tokens[i] = token;
		i++;
		/* grow the array if a line has more words than we planned for */
		if (i >= bufsize)
		{
			bufsize += 64;
			tokens = realloc(tokens, sizeof(char *) * bufsize);
			if (tokens == NULL)
			{
				perror("hsh");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, " \t\r\n");
	}
	tokens[i] = NULL;
	return (tokens);
}
