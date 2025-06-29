/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 00:21:08 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/30 00:45:05 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	cd_check(t_cmd *cmd, t_env *env)
{
	t_env	*env_loop;

	if (cmd->args[2])
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	if (!ft_strcmp(cmd->args[1], "-"))
	{
		env_loop = env;
		while (env_loop)
		{
			if (!ft_strcmp("OLDPWD", env_loop->key))
			{
				chdir(env_loop->value);
				printf("%s\n", env_loop->value);
				return (0);
			}
			env_loop = env_loop->next;
		}
	}
	else if (chdir(cmd->args[1]) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		file_not_exists(cmd->args[1]);
		return (1);
	}
	return (0);
}

int	change_dir(t_env *env_loop, char *value)
{
	if (env_loop->both)
		free(env_loop->both);
	if (env_loop->value)
		free(env_loop->value);
	env_loop->value = ft_strdup(value);
	if (!env_loop->value)
		return (free(value), g_exit_status = 12);
	env_loop->both = ft_strdup(env_loop->key);
	if (!env_loop->both)
		return (free(value), g_exit_status = 12);
	ft_strjoin_free(&env_loop->both, "=");
	ft_strjoin_free(&env_loop->both, env_loop->value);
	return (g_exit_status = 0);
}

int	take_pwd(t_env *env, char **temp)
{
	while (env)
	{
		if (!ft_strcmp("PWD", env->key))
		{
			*temp = ft_strdup(env->value);
			if (!*temp)
				return (g_exit_status = 12);
			break ;
		}
		env = env->next;
	}
	return (0);
}
