/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:30:29 by aorth             #+#    #+#             */
/*   Updated: 2025/06/29 12:25:53 by aorth            ###   ########.fr       */
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
		{
			if (env_loop->both)
				free(env_loop->both);
			if (env_loop->value)
				free(env_loop->value);
			env_loop->value = ft_strdup(pwd_value);
			if (!env_loop->value)
				return (free(pwd_value), g_exit_status = 12);
			env_loop->both = ft_strdup(env_loop->key);
			if (!env_loop->both)
				return (free(pwd_value), g_exit_status = 12);
			ft_strjoin_free(&env_loop->both, "=");
			ft_strjoin_free(&env_loop->both, env_loop->value);
			free(pwd_value);
			return (g_exit_status = 0);
		}
		env_loop = env_loop->next;
	}
	return (free(pwd_value), g_exit_status = 1);
}

int	cd_oldpwd(t_env **env)
{
	t_env	*env_loop;
	char	*temp;

	temp = NULL;
	env_loop = *env;
	while (env_loop)
	{
		if (!ft_strcmp("PWD", env_loop->key))
		{
			temp = ft_strdup(env_loop->value);
			if (!temp)
				return (g_exit_status = 12);
			break ;
		}
		env_loop = env_loop->next;
	}
	if (!temp)
		return (g_exit_status = 1);
	env_loop = *env;
	while (env_loop)
	{
		if (!ft_strcmp("OLDPWD", env_loop->key))
		{
			if (env_loop->both)
				free(env_loop->both);
			if (env_loop->value)
				free(env_loop->value);
			env_loop->value = ft_strdup(temp);
			if (!env_loop->value)
			{
				free(temp);
				return (g_exit_status = 12);
			}
			free(temp);
			env_loop->both = ft_strdup(env_loop->key);
			if (!env_loop->both)
				return (g_exit_status = 12);
			ft_strjoin_free(&env_loop->both, "=");
			ft_strjoin_free(&env_loop->both, env_loop->value);
			return (g_exit_status = 0);
		}
		env_loop = env_loop->next;
	}
	free(temp);
	return (g_exit_status = 1);
}

int	cd_check(t_cmd *cmd, t_env *env)
{
	t_env	*env_loop;

	if (cmd->args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
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
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
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
		else
		{
			cd_oldpwd(env);
			cd_pwd(env);
		}
		return (g_exit_status = 0);
	}
	return (g_exit_status = 0);
}
