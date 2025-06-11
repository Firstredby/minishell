/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:14:52 by codespace         #+#    #+#             */
/*   Updated: 2025/06/11 21:14:39 by ishchyro         ###   ########.fr       */
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

    if (!list || !*list)
        return ;
    i = 0;
    while (list[i])
        free(list[i++]);
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
    t_cmd   *current;
    t_cmd   *next;

    if (!cmd)
        return ;
    current = cmd;
    while (current)
    {
        next = current->next;
        if (current->cmd)
            free(current->cmd);
        free2d(current->args);
        free2d(current->limiter);
        closefd(current->fd);
        closefd(current->fd);
        closefd(current->fd);
        if (current->filename)
            free(current->filename);
        free(current);
        current = next;
    }
    cmd = NULL;
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
        free(curr);
        curr = next;
    }
    env = NULL;
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