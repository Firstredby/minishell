/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:02:34 by aorth             #+#    #+#             */
/*   Updated: 2025/06/29 23:59:07 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdio.h>
#include <sys/types.h>

int	alloc_export(t_env *env, int count)
{
	int		i;
	t_env	*env_loop;

	if (!env)
		return (0);
	env->exported_envs = ft_calloc(sizeof(char *), (count + 1));
	if (!env->exported_envs)
		return (perror("malloc:"), -1);
	i = 0;
	env_loop = env;
	while (env_loop)
	{
		env->exported_envs[i] = set_exported_env(env_loop);
		if (!env->exported_envs[i])
		{
			while (i >= 0)
				free(env->exported_envs[--i]);
			env->exported_envs = NULL;
			return (perror("malloc:"), -1);
		}
		env_loop = env_loop->next;
		i++;
	}
	return (0);
}

static void	sort_export(t_env *env, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env->exported_envs[j], env->exported_envs[j + 1]) > 0)
			{
				temp = env->exported_envs[j];
				env->exported_envs[j] = env->exported_envs[j + 1];
				env->exported_envs[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

int	export_replace(t_env *env_loop, t_cmd *cmd, char *temp, int index)
{
	if (env_loop->value)
		free(env_loop->value);
	if (env_loop->both)
		free(env_loop->both);
	env_loop->both = ft_strdup(cmd->args[index]);
	if (!env_loop->both)
		return (g_exit_status = 12);
	temp = env_strdup(cmd->args[index], false);
	if (!temp)
		return (g_exit_status = 12);
	if (*temp)
	{
		env_loop->value = ft_strdup(temp);
		if (!env_loop->value)
			return (g_exit_status = 12);
	}
	else
		env_loop->value = NULL;
	free(temp);
	return (g_exit_status = 0);
}

int	ft_export_add(t_cmd *cmd, t_env **env, int index)
{
	t_env	*env_loop;
	char	*temp;

	if (export_check(cmd, *env, index))
		return (g_exit_status = 1);
	env_loop = *env;
	temp = env_strdup(cmd->args[index], true);
	if (!temp)
		return (g_exit_status = 12);
	while (env_loop)
	{
		if (!ft_strcmp(temp, env_loop->key))
			if (export_replace(env_loop, cmd, temp, index))
				return (free(temp), 1);
		env_loop = env_loop->next;
	}
	free(temp);
	if (!cmd->next)
	{
		if (!(*env)->key)
			no_env_export(env, cmd, index);
		else
			env_add(env, cmd->args[index]);
	}
	return (g_exit_status = 0);
}

int	ft_export(t_env *env)
{
	int		count;
	t_env	*env_loop;

	if (!env)
		return (g_exit_status = 1);
	count = 0;
	env_loop = env;
	while (env_loop)
	{
		count++;
		env_loop = env_loop->next;
	}
	if (env->exported_envs)
	{
		free2d(env->exported_envs);
		env->exported_envs = NULL;
	}
	if (alloc_export(env, count) == -1)
		return (g_exit_status = 12);
	sort_export(env, count);
	return (g_exit_status = 0);
}
