#include "../../includes/minishell.h"
#include <stdio.h>

void	show_env(t_env *env)
{
	t_env *curr;

	curr = env;
	while (curr)
	{
		printf("env both: %s\n", curr->both);
		printf("env key: %s\n", curr->key);
		printf("env value: %s\n", curr->value);
		curr = curr->next;
	}
}

void	show_args(t_cmd *cmd)
{
	int l = 0;
	int	i = 0;
	while (cmd) //visualizer
	{
		printf("-------------------------------\n");
		printf("(cmd): %s\n", cmd->cmd);
		while (cmd->args[l])
		{
			printf("(arg[%d]): %s$\n", l, cmd->args[l]);
			l++;
		}
		printf("(fd): %d\n", cmd->fd);
		printf("(fd_in): %d\n", cmd->fd_in);
		printf("(fd_out): %d\n", cmd->fd_out);
		printf("(is_correct): %d\n", cmd->skip);
		printf("(filename): %s\n", cmd->filename);
		printf("limiter list pointer: %p\n", cmd->limiter);
		while (cmd->limiter[i])
		{
			printf("(limiter[%d]): %s\n", i, cmd->limiter[i]);
			i++;
		}
		printf("(next pointer): %p\n", cmd->next);
		l = 0;
		i = 0;
		cmd = cmd->next;
	}
	printf("----------end of list----------\n");
}

void	show_token(t_token **token)
{
	for (int i = 0; *(token + i);)
	{
		t_token *curr = token[i];
		while (curr)
		{
			printf("[%i] token is %s\n", i, curr->token);
			printf("[%i] token type is %d\n", i, curr->type);
			curr = curr->next;
		}
		i++;
	}
}