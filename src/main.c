/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/03/08 10:12:56 by aorth             #+#    #+#             */
/*   Updated: 2025/03/08 10:12:56 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int	g_exit_status = 0;

int	cmd_init(t_data *data, char *input, char **envp)
{
	if (!data->env)
		if (!env_handle(envp, &data->env))
			return (12);
	data->token = tokenizer(input, command_count(input));
	if (!data->token && g_exit_status == 12)
		return (12);
	else if (!data->token)
		return (1);
	if (parser_validator(data->token))
		return (free_all(NULL, NULL, data->token), data->token = NULL, 1);
	data->cmd = parser(data->token, data->env);
	if (!data->cmd)
		return (12);
	data->token = NULL;
	return (0);
}

int	start_exec(t_data *data)
{
	if (exe_prep(data->cmd))
		return (1);
	command_sigs();
	if (!data->cmd->next)
		exe_cmd(data->cmd, &data->env, data);
	else
		execute_pipe(data->cmd, data);
	return (0);
}

static int	main_cleaner(t_data *data, int check)
{
	int	should_exit;

	if (check == 12)
		return (free_all(data->cmd, NULL, NULL), g_exit_status = 12);
	should_exit = 0;
	if (data->cmd && data->cmd->cmd && !ft_strcmp(data->cmd->cmd, "exit"))
		should_exit = 1;
	if (start_exec(data))
		return (free_all(data->cmd, NULL, data->token), g_exit_status);
	cmd_cleaner(data->cmd);
	data->cmd = NULL;
	if (data->token)
	{
		trash_collector_goes_brrrr(data->token);
		data->token = NULL;
	}
	if (should_exit)
		return (0);
	else
		return (1);
}

int	main_loop(t_data *data, char **envp)
{
	int		check;
	char	*input;

	input = NULL;
	while (1)
	{
		main_sigs();
		input = readline("minishell$ ");
		if (input)
			add_history(input);
		if (!reality_check(input))
			return (printf("exit\n"), 0);
		if (reality_check(input) == 1)
			continue ;
		check = cmd_init(data, input, envp);
		if (input)
		{
			free(input);
			input = NULL;
		}
		if (check == 1)
			continue ;
		if (!main_cleaner(data, check))
			return (0);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	((void)argc, (void)argv);
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (12);
	data->cmd = NULL;
	data->env = NULL;
	data->token = NULL;
	main_loop(data, envp);
	free_all(NULL, data->env, NULL);
	free(data);
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	rl_clear_history();
	return (g_exit_status);
}
