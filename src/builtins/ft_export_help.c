/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_help.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:17:49 by aorth             #+#    #+#             */
/*   Updated: 2025/06/29 19:43:43 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	export_add_help(t_cmd *cmd, int index)
{
	int	i;

	i = 0;
	while (cmd->args[index][i] != '=' && cmd->args[index][i])
	{
		if (!ft_isalnum(cmd->args[index][i]))
		{
			if (cmd->args[index][i] != '_')
				return (1);
		}
		i++;
	}
	return (0);
}

int	export_check(t_cmd *cmd, t_env *env, int index)
{
	if (cmd->args[index][0] == '=' || export_add_help(cmd, index))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (g_exit_status = 1);
	}
	if (ft_strchr(cmd->args[index], '=') == 0)
	{
		if (env_add(&env, cmd->args[index]))
			return (0);
	}
	return (0);
}

void	no_env_export(t_env **env, t_cmd *cmd, int index)
{
	free(*env);
	env_add(env, cmd->args[index]);
	alloc_export(*env, 1);
}

char	*set_exported_env(t_env *env)
{
	if (!env->both)
		return (ft_strdup(env->key));
	else
		return (ft_strdup(env->both));
}
