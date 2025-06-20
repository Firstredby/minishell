/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:30:29 by aorth             #+#    #+#             */
/*   Updated: 2025/06/20 17:26:29 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_cd(t_cmd *cmd)
{
    if(!cmd->skip)
    {
        if (!cmd->args[1])
        {
            chdir(getenv("HOME"));
            return (g_exit_status = 0);
        }
        if (chdir(cmd->args[1]) == -1)
        {
            ft_putstr_fd("cd: ", 2);
            ft_putstr_fd(cmd->args[1], 2);
            ft_putstr_fd(": No such file or directory\n", 2);
            return (g_exit_status = 1);
        }
        return (g_exit_status = 0);
    }
    return (g_exit_status = 0);
}
