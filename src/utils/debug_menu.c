#include "../../includes/minishell.h"

void	show_args(t_cmd *cmd)
{
	int l = 0;
	while (cmd) //visualizer
	{
		printf("(cmd): %s\n", cmd->cmd);
		while (cmd->args[l])
		{
			printf("(arg[%d]): %s\n", l, cmd->args[l]);
			l++;
		}
		printf("(envp pointer): %p\n", cmd->env->envp);
		l = 0;
		cmd = cmd->next;
	}
}

void	show_token(char *input)
{
	t_token **token = tokenizerV3(input, command_count(input));
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