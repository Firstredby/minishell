/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:33:59 by aorth             #+#    #+#             */
/*   Updated: 2025/06/30 00:56:14 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

void	heredoc_helper(t_cmd *cmd)
{
	if (cmd->fd_in == 0 && *cmd->limiter)
	{
		cmd->fd_in = open(cmd->filename, O_RDONLY);
		if (cmd->fd_in == -1)
		{
			perror("open for read");
			return ;
		}
	}
}

int	sigs_help(t_cmd *cmd, char *line, int i)
{
	if (!line)
	{
		close(cmd->fd);
		unlink(cmd->filename);
		command_sigs();
		ft_putstr_fd("warning: here-document at line 2 delimited ", 2);
		ft_putstr_fd("by end-of-file (wanted `", 2);
		ft_putstr_fd(cmd->limiter[i], 2);
		ft_putstr_fd("')\n", 2);
		return (1);
	}
	if (!ft_strcmp(line, cmd->limiter[i]))
		return (2);
	return (0);
}

int	heredoc_loop(t_cmd *cmd, int index)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (sigs_help(cmd, line, index) == 1)
		{
			close(cmd->fd);
			return (free(line), 1);
		}
		else if (sigs_help(cmd, line, index) == 2)
			break ;
		write(cmd->fd, line, ft_strlen(line));
		write(cmd->fd, "\n", 1);
		free(line);
	}
	if (line)
		free(line);
	return (0);
}

int	handle_heredoc(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->limiter && cmd->limiter[i] && *cmd->limiter[i])
	{
		cmd->fd = open(cmd->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->fd != -1)
		{
			heredoc_sigs();
			if (heredoc_loop(cmd, i))
				return (1);
			command_sigs();
		}
		close(cmd->fd);
		i++;
	}
	heredoc_helper(cmd);
	return (0);
}

int	handle_redir(t_cmd *cmd)
{
	if (cmd->fd_out == -1 || cmd->fd_in == -1)
		return (g_exit_status = 1);
	if (cmd->fd_out != 0)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
	}
	if (cmd->fd_in != 0)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
	}
	return (0);
}
