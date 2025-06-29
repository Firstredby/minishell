/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:01:35 by aorth             #+#    #+#             */
/*   Updated: 2025/06/29 19:25:43 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	unset_seek(t_env **env, t_cmd *cmd)
{
	t_env	*env_temp;
	t_env	*env_loop;

	env_temp = NULL;
	env_loop = *env;
	while (env_loop)
	{
		if (env_loop->next && cmd->args[1]
			&& !ft_strcmp(cmd->args[1], env_loop->next->key))
		{
			free(env_loop->next->both);
			free(env_loop->next->key);
			free(env_loop->next->value);
			env_temp = env_loop->next;
			env_loop->next = env_loop->next->next;
			free(env_temp);
			env_temp = NULL;
		}
		env_loop = env_loop->next;
	}
}

static void	remove_first_node(t_env *node, t_env *origin)
{
	if (node->next)
	{
		node->next->exported_envs = node->exported_envs;
		node->exported_envs = NULL;
		origin = node->next;
	}
	else
	{
		free2d(node->exported_envs);
		origin = NULL;
	}
}

int	ft_unset(t_cmd *cmd, t_env **env)
{
	t_env	*env_loop;
	int		flag;
	int		i;

	flag = 0;
	i = 0;
	env_loop = *env;
	if (cmd && !cmd->next)
	{
		if (cmd->args[1] && env_loop && !ft_strcmp(cmd->args[1], env_loop->key))
		{
			remove_first_node(env_loop, *env);
			free(env_loop->both);
			free(env_loop->key);
			free(env_loop->value);
			free(env_loop);
			return (g_exit_status = 0);
		}
		unset_seek(env, cmd);
	}
	return (g_exit_status = 0);
}
