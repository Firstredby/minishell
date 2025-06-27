/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:11:45 by aorth             #+#    #+#             */
/*   Updated: 2025/06/27 15:42:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	echo_help(t_cmd *cmd)
{
	int		i;
	size_t	j;

	i = 1;
	while (cmd->args[i] && *cmd->args[i])
	{
		j = 0;
		if (!ft_strcmp(cmd->args[i], "-n"))
		{
			i++;
			continue ;
		}
		else if (cmd->args[i][j] == '-')
		{
			j++;
			while (cmd->args[i][j] == 'n')
				j++;
			if (ft_strlen(cmd->args[i]) > j)
				return (i);
		}
		else
			return (i);
		i++;
	}
	return (i);
}

int	ft_echo(t_cmd *cmd)
{
	int	i;
	int	n_flag;

	i = echo_help(cmd);
	n_flag = 0;
	if (i > 1)
		n_flag = 1;
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], 1);
		if (cmd->args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", 1);
	return (g_exit_status = 0);
}
