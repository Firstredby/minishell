/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 10:12:56 by aorth             #+#    #+#             */
/*   Updated: 2025/03/08 10:12:56 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

int	g_exit_status = 0;

int	cmd_init(t_data *data, char *input, char **envp)
{
	if (!data->env)
		if (!env_handle(envp, &data->env))
			return (free_all(NULL, data->env, NULL), 12);
	data->token = tokenizerV3(input, command_count(input));
	if (!data->token && g_exit_status == 12)
		return (free_all(NULL, data->env, data->token), 12);
	else if (!data->token)
		return (free_all(NULL, NULL, data->token), 1);
	if (parser_validator(data->token))
		return (free_all(NULL, NULL, data->token), 1);
	data->cmd = parserV3(data->token, data->env);
	if (!data->cmd)
		return (free_all(data->cmd, data->env, data->token), 12);
	return (0);
}

int	start_exec(t_data *data)
{
	if (exe_prep(data->cmd))
	{
		free_all(data->cmd, data->env, data->token);
		return (g_exit_status = 12);
	}
	command_sigs();
	if(!data->cmd->next)
		exe_cmd(data->cmd, &data->env);
	else
		execute_pipe(data->cmd, data->env);
	return (0);
}

int main(int argc, char **argv, char **envp)
{
    char	*input;
    t_data	*data;
	int		check;

    (void)argc;
    (void)argv;
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (12);
	data->cmd = NULL;
	data->env = NULL;
	data->token = NULL;
    while (1)
    {
		main_sigs();
        input = readline("minishell$ ");
		add_history(input);
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (input[0] == '\0')
        {
            free(input);
            continue;
        }
		check = cmd_init(data, input, envp);
		free(input);
		if (check == 1)
			continue ;
		if (check == 12)
			break ;
        if (start_exec(data))
			return (g_exit_status);
        cmd_cleaner(data->cmd);
		data->cmd = NULL;
		rl_clear_history();
	}
    free_all(NULL, data->env, NULL);
	free(data);
	if (input)
		free(input);
	rl_clear_history();
    return (g_exit_status);
}
