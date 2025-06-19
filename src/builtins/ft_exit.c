/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:39:59 by aorth             #+#    #+#             */
/*   Updated: 2025/06/18 19:45:54 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_exit(t_cmd *cmd)
{
    int i;

    i = 0;
    if (cmd->args[1])
    {
        if (cmd->args[2])
        {
            ft_putstr_fd("exit\nminishell: exit: too many arguments\n", 2);
            return (1);
        }
        while (cmd->args[1][i])
        {
            if (!ft_isdigit(cmd->args[1][i]) || ft_atoi(cmd->args[1]) > 9223372036854775807)
            {
                ft_putstr_fd("exit\n", 1);
                ft_putstr_fd("exit: ", 2);
                ft_putstr_fd(cmd->args[1], 2);
                ft_putstr_fd(": numeric argument required\n", 2);
                return (2);
            }
            i++;
        }
        return(0);//return (ft_atoi(cmd->args[1]));
    }
    return (0);
}
