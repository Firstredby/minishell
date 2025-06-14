/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:14:52 by codespace         #+#    #+#             */
/*   Updated: 2025/06/14 02:34:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
			free(current->token);
			free(current);
			current = next;
		}
		i++;
	}
	free(list);
}

void    free2d(char **list)
{
    int i;

    if (!list)
        return ;
    i = 0;
    while (*(list + i))
    {
        free(*(list + i));
        i++;
    }
    free(list);
    list = NULL;
}

void    closefd(int fd)
{
    if (fd > 0)
        close(fd);
}

void    cmd_cleaner(t_cmd *cmd)
{
    t_cmd   *next;

    while (cmd)
    {
        next = cmd->next;
        if (cmd->cmd)
            free(cmd->cmd);
        free2d(cmd->args);
        free2d(cmd->limiter);
        closefd(cmd->fd);
        closefd(cmd->fd);
        closefd(cmd->fd);
        if (cmd->filename)
            free(cmd->filename);
        free(cmd);
        cmd = next;
    }
}

void    env_cleaner(t_env **env)
{
    t_env   *curr;
    t_env   *next;

    if (!env)
        return ;
    curr = *env;
    while (curr)
    {
        next = curr->next;
        if (curr->both)
            free(curr->both);
        if (curr->value)
            free(curr->value);
        if (curr->key)
            free(curr->key);
        free(curr);
        curr = next;
    }
    env = NULL;
}

void    free_all(t_cmd *cmd, t_env **env, t_token **token)
{
    if (cmd)
        cmd_cleaner(cmd);
    if (env)
        env_cleaner(env);
    if (token)
        TRASH_COLLECTOR_GOES_BRRRR(token);
}