/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:01:35 by aorth             #+#    #+#             */
/*   Updated: 2025/05/14 19:12:06 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_unset(t_cmd *cmd)
{
    int i;
    int j;

    i = 0;
    while (cmd->args[i])
    {
        j = 0;
        while (cmd->env->envp[j])
        {
            if (ft_strcmp(cmd->args[i], cmd->env->envp[j]) == 0)
            {
                free(cmd->env->envp[j]);
                cmd->env->envp[j] = NULL;
            }
            j++;
        }
        i++;
    }
    return (0);
}
