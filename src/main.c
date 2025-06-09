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
        show_token(input);
		//cmds = parserV3(input, env);
        //printf("test0\n");
		show_token(input);
     
        //char *str[] = {"cat", "-e", "a"};
        
        // pid_t pid;
        // //char *str[] = {"cat", "-e", "a"};
        // pid = fork();
        // if (pid == 0)
        // {
        //     execvp(cmds->cmd, cmds->args);
        //     exit(EXIT_FAILURE);
        // }
        // waitpid(pid, NULL, 0);
        // execvp(cmds->cmd, cmds->args);
        //exe_cmd(cmds);
        // t_cmd *cmd_loops = cmds;
        // // while (cmds)
        // // {
        // //     //printf("cmd:%s\n", cmds->cmd);
        // //     cmds = cmds->next;
        // //     printf("Original pointer:%p\n", cmds);
        // // }
        // while (cmd_loops)
        // {
        //     cmd_loops = cmd_loops->next;
        //     printf("pointer:%p\n", cmd_loops);
        // }
        // continue;
        exe_prep(cmds);
        // int i_loop = 1;
        
        // while (cmd_loops)
        // {
        //     cmd_loops->node_nbr = i_loop;
        //     cmd_loops->filename = create_filename("/tmp/heredoc",ft_itoa(cmd_loops->node_nbr), ".tmp");
        //     // if (!cmd_loops->filename)
        //     //     {
        //     //         free()
        //     //     }
        //     handle_heredoc(cmd_loops);
        //     i_loop++;
        //     cmd_loops = cmd_loops->next;
        // }
        show_args(cmds);
        if(!cmds->next)
            exe_cmd(cmds);
        else
            execute_pipe2(cmds);
        /*
         Need a soultion for the fds. I need the correct order, or we have to execute heredoc in the parser part
        somehow and then just pass me the last fd. Heredoc function itself must be executed before the pipe calling.
        The redirection of it musst be done after calling the pipes so it can overwrite it if needed.
        In commands like "> b cat -e < c << test | grep s > a" the "< c" is not noticed as a redirection because the fd_in is 0.
		*/
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
