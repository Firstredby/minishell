/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirv2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:33:59 by aorth             #+#    #+#             */
/*   Updated: 2025/06/03 21:29:20 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

void    handle_redirV2(t_cmd *cmd)
{
    char *line;
    
    if (cmd->limiter)
    {
        cmd->fd = open("/tmp/heredoc.tmp", O_WRONLY | O_CREAT | O_TRUNC , 0644);
        if (cmd->fd != -1)
        {
            printf("sdadsa\n");
            while(1)
            {
                line = readline("> s");
                if (!line || !ft_strcmp(line, cmd->limiter))
                    break;
                write(cmd->fd, line, ft_strlen(line));
                write(cmd->fd, "\n", 1);
                free(line);
            }
        }
        dup2(cmd->fd, 0);
        close(cmd->fd);    
        cmd->fd = open("/tmp/heredoc.tmp", O_RDONLY);
        if (cmd->fd == -1)
        {
            perror("open for read");
            return;
        }
        dup2(cmd->fd, STDIN_FILENO);
        close(cmd->fd);
    }
    // dup2(cmd->fd, 0);
    // close (cmd->fd);
    if (cmd->fd_out == -1 || cmd->fd_in == -1)
        printf("No such file or directory\n");
    if (cmd->fd_out != 0)
    {
        dup2(cmd->fd_out, 1);
        close(cmd->fd_out);
    }
    if (cmd->fd_in != 0)
    {
        dup2(cmd->fd_in, 0);
        close(cmd->fd_in);
    }
}
