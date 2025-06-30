/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_it_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 01:48:17 by aorth             #+#    #+#             */
/*   Updated: 2025/06/30 02:09:36 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>
#include <sys/types.h>

void	init_tpipe(t_cmd *cmd)
{
	t_cmd	*current;
	t_pipe	*pipe;

	pipe = ft_calloc(sizeof(t_pipe), 1);
	if (!pipe)
		return (perror("malloc"));
	pipe->cmd_count = 0;
	pipe->pipe_count = 0;
	current = cmd;
	while (current)
	{
		current->pipe = pipe;
		current = current->next;
	}
}

void	skip_broken_commands(t_cmd *cmd)
{
	t_cmd	*curr;

	curr = cmd;
	while (curr)
	{
		if (curr->fd_in == -1 || curr->fd_out == -1)
			curr->skip = true;
		curr = curr->next;
	}
}

void	pipe_exit_status(int status)
{
	int	sig;

	sig = 0;
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
}

int	alloc_pipe_help(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->pipe->pipe_count)
	{
		cmd->pipe->fds[i] = malloc(sizeof(int) * 2);
		if (cmd->pipe->fds[i] == NULL)
		{
			while (--i >= 0)
				free(cmd->pipe->fds[i]);
			free(cmd->pipe->fds);
			cmd->pipe->fds = NULL;
			return (perror("malloc"), -1);
		}
		if (pipe(cmd->pipe->fds[i]) == -1)
		{
			while (i >= 0)
				free(cmd->pipe->fds[i--]);
			free(cmd->pipe->fds);
			cmd->pipe->fds = NULL;
			return (perror("pipe"), -1);
		}
		i++;
	}
	return (0);
}
