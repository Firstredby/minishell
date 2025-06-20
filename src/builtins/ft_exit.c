/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:39:59 by aorth             #+#    #+#             */
/*   Updated: 2025/06/20 20:40:11 by ishchyro         ###   ########.fr       */
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



int ft_exit(t_cmd *cmd)
{
    int i;

    i = 0;
    if (cmd->args[1])
    {
		if (!cmd->args[1][0])
			return (err_msg(cmd), 2);
		while (cmd->args[1][i] == '0')
			i++;
		if (ft_strlen(&cmd->args[1][i]) > 20)
			return (2);
        while (cmd->args[1][i])
		{
			if (cmd->args[1][i] == '+' || cmd->args[1][i] == '-')
				++i;
			if (!ft_isdigit(cmd->args[1][i++])
				|| ft_atoi(cmd->args[1]) > 9223372036854775807)
				return (err_msg(cmd), 2);
		}
        if (cmd->args[2])
            return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
		return (ft_atoi(cmd->args[1]));
    }
    return (0);
}
