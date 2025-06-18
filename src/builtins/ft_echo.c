/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:11:45 by aorth             #+#    #+#             */
/*   Updated: 2025/06/18 17:58:49 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int    ft_echo(t_cmd *cmd)
{
    int i;
    int n_flag;

    i = 1;
    n_flag = 0;
    while (cmd->args[i] && !ft_strcmp(cmd->args[i], "-n"))
    {
        n_flag = 1;
        i++;
    }
    while (cmd->args[i])
    {
        ft_putstr_fd(cmd->args[i], 1);
        if (cmd->args[i + 1])
            ft_putstr_fd(" ", 1);
        i++;
    }
    if (!n_flag)
        ft_putstr_fd("\n", 1);
    return (0);
}
