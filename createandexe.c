#include "headersh.h"

/**
 * free_tok - frees the @tokens string
 * @tokens: string containing the parsed input line
 * Return: nothing
 */
void free_tok(char ***tokens)
{
	free((*tokens)[0]);
}


/**
  * check_command - creat and execute the command given by user
  * @tokens: strings from stdin
  * @cc: is the counter of commans executes by user
  * @en: list containing the end parameter for execve syscall
  * @av: list containing the arguments given by user
  * Return: the process status
  */

int check_command(char ***tokens, int *cc, char **en, char **av)
{
	int statu = 0;
	char **buffer = *tokens, *tok;

	statu = built_ins_sh(tokens, en, buffer);
	if (statu != 0)
		return (2);
	statu = add_path(tokens, en);
	if (statu != 0 && statu != 1)
		return (2);
	tok = (*tokens)[0];
	if (access(tok,  F_OK | X_OK) == 0)
	{
		return (statu);
	}
	else
	{
		if (access(tok,  F_OK) != 0)
		{
			dprintf(STDERR_FILENO, "%s: %d: %s: not found\n", av[0], *cc, tok);
		}
		else if (access(tok, X_OK) != 0)
		{
			dprintf(STDERR_FILENO, "%s: %d: %s: Permission denied\n", av[0], *cc, tok);
		}
		return (127);
	}
	return (0);
}

/**
  * createandexesh - creat and execute the command given by user
  * @tokens: strings from stdin
  * @cc: is the counter of commans executes by user
  * @en: list containing the end parameter for execve syscall
  * @av: list containing the arguments given by user
  * Return: the process status
  */
int createandexesh(char ***tokens, int *cc, char **en, char **av)
{
	pid_t child_pid;
	int wait_status = 0, statu = 0;
	char **buffer = *tokens;

	statu = check_command(tokens, cc, en, av);
	if (statu != 0 && statu != 1)
		return (statu);

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error:");
		return (1);
	}
	if (child_pid == 0)
	{
		if (execve((*tokens)[0], (*tokens), en) == -1)
		{
			if (statu == 1)
				free_tok(tokens);
			dprintf(STDERR_FILENO, "%s: %d: %s: not found\n", av[0], *cc, (*tokens)[0]);
			free_all(buffer, tokens);
			return (127);
		}
	}
	else
		wait(&wait_status);

	if (statu == 1)
		free_tok(tokens);

	return (0);
}
