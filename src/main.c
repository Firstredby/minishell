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


/* Free a split array */
void ft_free_split(char **split)
{
    int i = 0;
    
    if (!split)
        return;
    
    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
}


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
                exit(12);
            data.env = env;
        }
        token = tokenizerV3(input, command_count(input));
        if (!token)
            (env_cleaner(env), exit(12));
        data.token = token;
		//show_token(token);
        cmds = parserV3(token, env);
        if (!cmds)
            (free_all(NULL, env, token), exit(12));
        data.cmd = cmds;
        //show_args(cmds);
        (void) data;
        exe_prep(cmds);
        if(!cmds->next)
            exe_cmd(cmds, env);
        else
            execute_pipe2(cmds);
        free(input);
        cmd_cleaner(cmds);
}
    free_all(NULL, env, NULL);
	rl_clear_history();
    return (g_exit_status);
}
