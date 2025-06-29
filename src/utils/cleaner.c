/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:14:52 by codespace         #+#    #+#             */
/*   Updated: 2025/06/29 22:35:10 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	trash_collector_goes_brrrr(t_token **list)
{
	int		i;
	t_token	*current;
	t_token	*next;

	i = 0;
	if (!list)
		return ;
	while (list[i])
	{
		current = list[i];
		while (current)
		{
			next = current->next;
			if (current->token)
			{
				free(current->token);
				current->token = NULL;
			}
			free(current);
			current = next;
		}
		list[i] = NULL;
		i++;
	}
	free(list);
}

void	cmd_cleaner(t_cmd *cmd)
{
	t_cmd	*next;
	t_pipe	*pipe_to_free;

	pipe_to_free = NULL;
	while (cmd)
	{
		next = cmd->next;
		free(cmd->cmd);
		free2d(cmd->args);
		free2d(cmd->limiter);
		closefd(cmd->fd_in);
		closefd(cmd->fd_out);
		closefd(cmd->fd);
		free(cmd->filename);
		if (cmd->pipe && pipe_to_free != cmd->pipe)
		{
			pipe_to_free = cmd->pipe;
			pipe_cleaner(cmd->pipe);
		}
		free(cmd);
		cmd = next;
	}
}

void	env_cleaner(t_env *env)
{
	t_env	*curr;
	t_env	*next;

	if (!env)
		return ;
	curr = env;
	while (curr)
	{
		next = curr->next;
		if (curr->both)
			free(curr->both);
		if (curr->value)
			free(curr->value);
		if (curr->key)
			free(curr->key);
		if (curr->exported_envs)
			free2d(curr->exported_envs);
		free(curr);
		curr = next;
	}
}

void	pipe_cleaner(t_pipe *pipe)
{
	int	i;

	if (!pipe)
		return ;
	if (pipe->fds)
	{
		i = 0;
		while (i < pipe->pipe_count)
		{
			if (pipe->fds[i])
				free(pipe->fds[i]);
			i++;
		}
		free(pipe->fds);
	}
	if (pipe->pid)
		free(pipe->pid);
	free(pipe);
}

void	child_safe_cleanup(t_cmd *cmd)
{
	t_cmd	*next;
	t_pipe	*pipe_to_free;

	if (!cmd)
		return ;
	pipe_to_free = NULL;
	while (cmd)
	{
		next = cmd->next;
		(free(cmd->cmd), free(cmd->filename));
		(free2d(cmd->args), free2d(cmd->limiter));
		closefd(cmd->fd_in);
		closefd(cmd->fd_out);
		closefd(cmd->fd);
		if (cmd->pipe && pipe_to_free != cmd->pipe)
		{
			pipe_to_free = cmd->pipe;
			pipe_cleaner(cmd->pipe);
			cmd->pipe = NULL;
		}
		free(cmd);
		cmd = next;
	}
}
