/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:30:29 by aorth             #+#    #+#             */
/*   Updated: 2025/06/30 00:43:48 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	cd_pwd(t_env **env)
{
	t_env	*env_loop;
	char	*pwd_value;

	env_loop = *env;
	pwd_value = getcwd(NULL, 0);
	if (!pwd_value)
		return (g_exit_status = 1);
	while (env_loop)
	{
		if (!ft_strcmp("PWD", env_loop->key))
			if (change_dir(env_loop, pwd_value))
				return (12);
		env_loop = env_loop->next;
	}
	return (free(pwd_value), g_exit_status = 0);
}

int	cd_oldpwd(t_env **env)
{
	t_env	*env_loop;
	char	*temp;

	env_loop = *env;
	if (take_pwd(env_loop, &temp))
		return (1);
	if (!temp)
		return (g_exit_status = 1);
	env_loop = *env;
	while (env_loop)
	{
		if (!ft_strcmp("OLDPWD", env_loop->key))
			if (change_dir(env_loop, temp))
				return (free(temp), 12);
		env_loop = env_loop->next;
	}
	return (free(temp), g_exit_status = 0);
}

int	ft_cd(t_cmd *cmd, t_env **env)
{
	if (!cmd->skip)
	{
		if (!cmd->args[1])
		{
			chdir(getenv("HOME"));
			return (g_exit_status = 0);
		}
		if (cd_check(cmd, *env))
			return (g_exit_status = 1);
		else if (cd_oldpwd(env) || cd_pwd(env))
			return (1);
		return (g_exit_status = 0);
	}
	return (g_exit_status = 0);
}
