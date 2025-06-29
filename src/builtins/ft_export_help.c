/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_help.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:17:49 by aorth             #+#    #+#             */
/*   Updated: 2025/06/29 13:37:17 by aorth            ###   ########.fr       */
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
