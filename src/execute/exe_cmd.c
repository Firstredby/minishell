/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:42:59 by aorth             #+#    #+#             */
/*   Updated: 2025/06/03 20:52:53 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


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
        }
        else
        {
            execvp(cmd->cmd, cmd->args);
        }
        dup2(cmd->fd_in, STDIN_FILENO);
        dup2(cmd->fd_out, STDOUT_FILENO);
        close(cmd->fd_in);
        close(cmd->fd_out);
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
