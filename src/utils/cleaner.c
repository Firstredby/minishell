/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:14:52 by codespace         #+#    #+#             */
/*   Updated: 2025/06/25 00:07:16 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	TRASH_COLLECTOR_GOES_BRRRR(t_token **list)
{
	int i = 0;
	t_token *current;
	t_token *next;

	if (!list)
		return;
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

void    free2d(char **list)
{
    int i;

    if (!*list)
        return (free(list));
    i = 0;
    while (list[i])
    {
        free(list[i]);
        list[i] = NULL;
        i++;
    }
    free(list[i]);
    free(list);
}

void    closefd(int fd)
{
    if (fd > 2)
        close(fd);
}

void    cmd_cleaner(t_cmd *cmd)
{
    t_cmd   *next;
    t_pipe  *pipe_to_free = NULL;

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

void    env_cleaner(t_env *env)
{
    t_env   *curr;
    t_env   *next;

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

void    pipe_cleaner(t_pipe *pipe)
{
    int i;

    if (!pipe)
        return;
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

void    free_all(t_cmd *cmd, t_env *env, t_token **token)
{
    if (cmd)
        cmd_cleaner(cmd);
    if (env)
        env_cleaner(env);
    if (token)
        TRASH_COLLECTOR_GOES_BRRRR(token);
}

void    child_safe_cleanup(t_cmd *cmd)
{
    t_cmd   *current;
    t_cmd   *next;
    t_pipe  *pipe_to_free = NULL;

    if (!cmd)
        return;
    current = cmd;
    while (current)
    {
        next = current->next;
        if (current->cmd)
        {
            free(current->cmd);
            current->cmd = NULL;
        }
        if (current->args)
        {
            free2d(current->args);
            current->args = NULL;
        }
        if (current->limiter)
        {
            free2d(current->limiter);
            current->limiter = NULL;
        }
        if (current->filename)
        {
            free(current->filename);
            current->filename = NULL;
        }
        closefd(current->fd_in);
        closefd(current->fd_out);
        closefd(current->fd);
        if (current->pipe && pipe_to_free != current->pipe)
        {
            pipe_to_free = current->pipe;
            pipe_cleaner(current->pipe);
            current->pipe = NULL;
        }
        free(current);
        current = next;
    }
    if(STDOUT_FILENO)
        close(STDOUT_FILENO);
    if(STDIN_FILENO)
        close(STDIN_FILENO);
    }
