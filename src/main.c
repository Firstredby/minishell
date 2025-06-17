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

int g_exit_status = 0;

int main(int argc, char **argv, char **envp)
{
    char *input;
    //char **args = NULL;
   // t_cmd *cmd = NULL;
   // t_cmd *cmd1 = NULL;
   // t_cmd *cmd2 = NULL;
    t_env *env = NULL;
    t_cmd *cmds = NULL;
    t_token **token = NULL;
    t_data  data;

    (void)argc;
    (void)argv;

    while (1)
    {
		// Display prompt and read line
        input = readline("minishell$ ");
		add_history(input);
		
        // Handle EOF (Ctrl+D)
        if (!input)
        {
            printf("exitss\n");
            break;
        }
        
        // Skip empty lines
        if (input[0] == '\0')
        {
            free(input);
            continue;
        }
        if (!env)
        {
            if (!env_handle(envp, &env))
                continue ;
            data.env = env;
        }
        token = tokenizerV3(input, command_count(input));
        if (!token)
		{
            free_all(NULL, NULL, token);
			continue ;
		}
        data.token = token;
		//show_token(token);
        //continue ;
        cmds = parserV3(token, env);
        if (!cmds || g_exit_status != 0)
		{
            (free_all(cmds, NULL, NULL));
			continue ;
		}
        data.cmd = cmds;
        //show_args(cmds);
        (void) data;
		if (!g_exit_status)
		{
			exe_prep(cmds);
			if(!cmds->next)
				exe_cmd(cmds, env);
			else
				execute_pipe(cmds, env);
		}
        free(input);
        cmd_cleaner(cmds);
}
    free_all(NULL, env, NULL);
	rl_clear_history();
    return (g_exit_status);
}
