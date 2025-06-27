/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:39:59 by aorth             #+#    #+#             */
/*   Updated: 2025/06/27 15:34:44 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	err_msg(t_cmd *cmd)
{
	ft_putstr_fd("exit\n", 1);
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(cmd->args[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

int	value_validator(t_cmd *cmd)
{
	int	pos;
	int	len;
	int	negative;

	pos = 0;
	negative = 0;
	if (cmd->args[1][pos] == '-')
	{
		negative = 1;
		pos++;
	}
	while (cmd->args[1][pos] == '0' || cmd->args[1][pos] == ' '
		|| (cmd->args[1][pos] == '+' && pos == 0))
		pos++;
	len = ft_strlen(&cmd->args[1][pos]);
	if (len > 20)
		return (err_msg(cmd), g_exit_status = 2);
	else if (len == 19 && negative
		&& ft_strcmp(&cmd->args[1][pos], "9223372036854775808") > 0)
		return (err_msg(cmd), g_exit_status = 2);
	else if (len == 19 && !negative
		&& ft_strcmp(&cmd->args[1][pos], "9223372036854775807") > 0)
		return (err_msg(cmd), g_exit_status = 2);
	else
		return (0);
}

int	ft_exit(t_cmd *cmd)
{
	int	exit_code;
	int	i;

	i = 0;
	if (cmd->args[1])
	{
		if (!cmd->args[1][0])
			return (err_msg(cmd), 2);
		if (value_validator(cmd))
			return (g_exit_status);
		while (cmd->args[1][i])
		{
			if (!ft_isdigit(cmd->args[1][i]) && cmd->args[1][i] != '+'
				&& cmd->args[1][i] != '-' && cmd->args[1][i] != ' ')
				return (err_msg(cmd), g_exit_status = 2);
			i++;
		}
		if (cmd->args[2])
			return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
		exit_code = ft_atoi(cmd->args[1]);
		if (exit_code == 2)
			err_msg(cmd);
		return (g_exit_status);
	}
	return (0);
}
