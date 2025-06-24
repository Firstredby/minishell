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
		return (free_all(NULL, data->env, NULL), 12);
	else if (!data->token)
		return (1);
	if (parser_validator(data->token))
		return (free_all(NULL, NULL, data->token), data->token = NULL, 1);
	data->cmd = parserV3(data->token, data->env);
	if (!data->cmd)
		return (free_all(data->cmd, data->env, NULL), 12);
	data->token = NULL;
	//show_args(data->cmd);
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
		exe_cmd(data->cmd, &data->env, data);
	else
		execute_pipe(data->cmd, data->env, data);
	return (0);
}

int main(int argc, char **argv, char **envp)
{
    char	*input;
    t_data	*data;
	int		check;
	int		should_exit;

    (void)argc;
    (void)argv;
	should_exit = 0;
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (12);
	data->cmd = NULL;
	data->env = NULL;
	data->token = NULL;
    input = NULL;
    while (1)
    {
		main_sigs();
        input = readline("minishell$ ");
		if (input)
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
        input = NULL;
		if (check == 1)
			continue ;
		if (check == 12)
		{
		    if (data->cmd)
		    {
		        cmd_cleaner(data->cmd);
		        data->cmd = NULL;
		    }
		    if (data->token)
		    {
		        TRASH_COLLECTOR_GOES_BRRRR(data->token);
		        data->token = NULL;
		    }
			break ;
		}
        if (data->cmd && data->cmd->cmd && !ft_strcmp(data->cmd->cmd, "exit"))
            should_exit = 1;
        if (start_exec(data))
        {
            free_all(data->cmd, data->env, data->token);
            free(data);
			return (g_exit_status);
        }
		//printf("minishell: %d\n", g_exit_status);
        cmd_cleaner(data->cmd);
		//printf("minishell1: %d\n", g_exit_status);
		data->cmd = NULL;
        if (data->token)
        {
            TRASH_COLLECTOR_GOES_BRRRR(data->token);
            data->token = NULL;
        }
		//printf("minishell2: %d\n", g_exit_status);
		if (should_exit)
		    break;
		//show_args(data->cmd);
	}
    free_all(NULL, data->env, NULL);
	free(data);
	rl_clear_history();
    return (g_exit_status);
}
