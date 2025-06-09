/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:07:47 by aorth             #+#    #+#             */
/*   Updated: 2025/06/03 13:34:15 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    redir_append(t_cmd *cmd, int i)
{
    int fd;

    if (!ft_strcmp(cmd->args[i], ">>"))
    {
        fd = open(cmd->args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->args[i + 1], 2);
            ft_putstr_fd(": No such file or directory\n", 2);
            exit(1);
        }
        dup2(fd, 1);
        close(fd);
        cmd->args[i] = NULL;
        cmd->args[i + 1] = NULL;
    }
}
void    redir_out(t_cmd *cmd, int i)
{
    int fd;

    if (!ft_strcmp(cmd->args[i], ">"))
    {
        fd = open(cmd->args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->args[i + 1], 2);
            ft_putstr_fd(": No such file or directory\n", 2);
            exit(1);
        }
        dup2(fd, 1);
        close(fd);
        cmd->args[i] = NULL;
        cmd->args[i + 1] = NULL;
    }
}

void    redir_in(t_cmd *cmd, int i)
{
    int fd;

    if (!ft_strcmp(cmd->args[i], "<"))
    {
        fd = open(cmd->args[i + 1], O_RDONLY);
        if (fd == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->args[i + 1], 2);
            ft_putstr_fd(": No such file or directory\n", 2);
            exit(1);
        }
        dup2(fd, 0);
        close(fd);
        cmd->args[i] = NULL;
        cmd->args[i + 1] = NULL;
    }
}

void    handle_redir(t_cmd *cmd)
{
    int i;
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stdin = dup(STDIN_FILENO); 
    
    i = 0;
    while (cmd->args[i])
    {
        if (!ft_strcmp(cmd->args[i], ">>"))
            redir_append(cmd, i);
        else if (!ft_strcmp(cmd->args[i], ">"))
            redir_out(cmd, i);
        else if (!ft_strcmp(cmd->args[i], "<"))
            redir_in(cmd, i);
        i++;
    }
    cmd->fd_in = saved_stdin;
    cmd->fd_out = saved_stdout;
}
