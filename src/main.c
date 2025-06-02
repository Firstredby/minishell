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

/* Duplicate a string */
// char *ft_strdup(const char *s)
// {
//     char *dup;
//     size_t len;
    
//     len = ft_strlen(s);
//     dup = (char *)malloc(sizeof(char) * (len + 1));
//     if (!dup)
//         return (NULL);
    
//     size_t i = 0;
//     while (i < len)
//     {
//         dup[i] = s[i];
//         i++;
//     }
//     dup[i] = '\0';
//     return (dup);
// }

/* Count the number of words in a string */


/* Split string into array of strings */


int main(int argc, char **argv, char **envp)
{
    char *input;
    //char **args = NULL;
   // t_cmd *cmd = NULL;
   // t_cmd *cmd1 = NULL;
   // t_cmd *cmd2 = NULL;
    t_env *env = NULL;
    t_cmd *cmds = NULL;
    

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
       t_env *env_head = NULL;
        env_handle(envp, &env);
       // env = env_head;
		cmds = parserV3(input, env);
        //printf("test0\n");
		//show_token(input);
        show_args(cmds);
		continue ;
		////////////////////////////////////////////////////////////
        //cmds = parser(input, env);
        //printf("test\n");
        t_env *env_loop = NULL;

        t_cmd *cmd_loop = cmds;
        while (cmd_loop)
        {
            int l = 0;
            while(cmd_loop->args[l])
            {
                env_loop = env_head;
                while(env_loop->next)
                {
                    if (!(env_strcmp(cmd_loop->args[l], env_loop->key)))
                    {
                        free (cmd_loop->args[l]);
                        cmd_loop->args[l] = ft_strdup(env_loop->value);
						//printf("%s", cmd_loop->args[l]);
                        break;
                    }
                    env_loop = env_loop->next;
                }
                l++;
            }
            cmd_loop = cmd_loop->next;
        }
        if (!cmds)
        {
            printf("error\n");
            free(input);
            continue;
        }
        if (ft_strcmp(cmds->cmd, "exit") == 0)
        {
            free(input);
            ll_free(cmds);
            printf("exit\n");
            exit(1);
        }
        if (cmds->next == NULL)
            exe_cmd(cmds);
        else
        {
            execute_pipe2(cmds);
            // while (cmds)
            // {
            //     //printf("cmd222: %s\n", cmds->cmd);
            //     if (cmds->next == NULL)
            //     {
            //         printf("cmd1: %s\n", cmds->cmd);
            //         //exe_cmd(cmds);
            //         //printf("cmd2: %s\n", cmds->cmd);
            //         break;
            //     }
            //     else
            //     {
            //         printf("cmd: %s\n", cmds->cmd);
            //         execute_pipe(cmds, cmds->next);
            //         printf("cmd2: %s\n", cmds->next->cmd);
            //         cmds = cmds->next;
            //     }
            //     //cmds = cmds->next;
            // }
        }
		
        ll_free(cmds);
		continue ; //cut
        
}
	rl_clear_history();
    return (g_exit_status);
}
