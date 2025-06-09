/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:42:59 by aorth             #+#    #+#             */
/*   Updated: 2025/06/06 18:09:27 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void    exe_prep(t_cmd *cmd)
{
    int i_loop;

    i_loop = 1;
    while (cmd)
    {
        cmd->node_nbr = i_loop;
        cmd->filename = create_filename("/tmp/heredoc",ft_itoa(cmd->node_nbr), ".tmp");
        // if (!cmd->filename)
        //     {
        //         free()
        //     }
        handle_heredoc(cmd);
        i_loop++;
        cmd = cmd->next;
    }
}

void    exe_cmd(t_cmd *cmd)
{
    pid_t pid;
    
    pid = fork();
    if (pid == 0)
    {
        handle_redirV2(cmd);
        if (is_builtin(cmd))
        {
            run_builtin(cmd);
            exit(0);
        }
        else
        {
            execvp(cmd->cmd, cmd->args);
        }
        printf("%d finished\n", pid);
        exit(EXIT_FAILURE);
    }
    else
    {
        waitpid(pid, NULL, 0);
        printf("Child process %d finished\n", pid);
         if (cmd->fd_in > 2) close(cmd->fd_in);
         if (cmd->fd_out > 2) close(cmd->fd_out);
         if (cmd->fd > 2) close(cmd->fd_out);
    }
}
