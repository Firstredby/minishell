/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:42:59 by aorth             #+#    #+#             */
/*   Updated: 2025/06/19 21:59:28 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>


void    exe_prep(t_cmd *cmd)
{
    int i_loop;

    i_loop = 1;
    while (cmd)
    {
        cmd->node_nbr = i_loop;
        cmd->filename = create_filename("/tmp/heredoc", ft_itoa(cmd->node_nbr), ".tmp");
        // if (!cmd->filename)
        //     {
        //         free()
        //     }
        
        handle_heredoc(cmd);
        i_loop++;
        cmd = cmd->next;
    }
}

void    exe_help(int status, t_cmd *cmd, pid_t pid)
{
    int sig;

    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        g_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
    {
        sig = WTERMSIG(status);
        if (sig == SIGINT)
            g_exit_status = 130;
        else if (sig == SIGQUIT)
        {
            g_exit_status = 131;
            printf("Quit (core dumped)\n");
        }
    }
    //printf("Exit status: %d\n", g_exit_status);
    if (cmd->fd_in > 2) close(cmd->fd_in);
    if (cmd->fd_out > 2) close(cmd->fd_out);
    if (cmd->fd > 2) close(cmd->fd_out);
}


void    exe_cmd(t_cmd *cmd, t_env *env)
{
    pid_t pid;
    int status = 0;
    
    builtin_parent(cmd, env);
    pid = fork();
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
		handle_redirV2(cmd);
        if(cmd->cmd && is_builtin(cmd))
        {
		    run_builtin(cmd, env);
            exit(g_exit_status);    
        }
        else 
		{
            if(execvp(cmd->cmd, cmd->args) == -1)
            {
				undef_cmd(cmd->cmd);
				exit(g_exit_status);
            }
		}
    }
    else if (pid > 0)
        exe_help(status, cmd, pid);
    else
        perror("Fork failed");
}
