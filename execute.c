#include "shell.h"
#include <errno.h>

/**
 * command_not_found - report a failed execve the way sh does
 * @args: NULL-terminated words; args[0] is the command that failed
 * @prog_name: our shell's name (argv[0])
 * @line_number: number of the input line this command came from
 *
 * sh reports ENOENT/ENOTDIR as "not found" instead of using the raw
 * strerror text, so that special case is matched here too.
 */
void command_not_found(char **args, char *prog_name, int line_number)
{
	if (errno == ENOENT || errno == ENOTDIR)
		fprintf(stderr, "%s: %d: %s: not found\n",
			prog_name, line_number, args[0]);
	else
		fprintf(stderr, "%s: %d: %s: %s\n",
			prog_name, line_number, args[0], strerror(errno));
	free(args); /* execve failed, so this child still owns args */
	exit(127);
}

/**
 * execute - run one command in a child process and wait for it
 * @args: NULL-terminated words; args[0] is the command, the rest are
 *        its arguments (passed straight to the program)
 * @prog_name: our shell's name (argv[0]), printed when a command fails
 * @line_number: number of the input line this command came from
 *
 * For this version the command must be a full path (e.g. /bin/ls);
 * searching PATH is a later task. We fork so the child can be replaced
 * by the new program while the shell (the parent) keeps running.
 *
 * Return: the command's exit status, so the shell can exit with the
 * same status as sh does when it reaches end-of-file
 */
int execute(char **args, char *prog_name, int line_number)
{
	pid_t pid;
	int status = 0;

	pid = fork();
	if (pid == -1) /* fork itself failed: report and keep the shell alive */
	{
		perror(prog_name);
		return (1);
	}
	if (pid == 0)
	{
		/* child: replace this process image with the requested program */
		if (execve(args[0], args, environ) == -1)
			command_not_found(args, prog_name, line_number);
	}
	else
	{
		/* parent: pause here until this child finishes */
		waitpid(pid, &status, 0);
	}
	return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}
