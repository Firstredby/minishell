/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboxuser <vboxuser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:01:35 by aorth             #+#    #+#             */
/*   Updated: 2025/06/24 03:20:16 by vboxuser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_unset(t_cmd *cmd, t_env **env)
{
    t_env *env_temp;
    t_env *env_loop;

    env_temp = NULL;
    env_loop = *env;
    if(cmd && !cmd->next)
    {
        while (env_loop)
        {
            if (env_loop->next && cmd->args[1] && ft_strcmp(cmd->args[1], env_loop->next->key) == 0)
            {
                free(env_loop->next->both);
                free(env_loop->next->key);
                free(env_loop->next->value);
                env_temp = env_loop->next;
                env_loop->next = env_loop->next->next;
                free(env_temp);
                env_temp = NULL;
            }
            env_loop = env_loop->next;
        }
    }
    return (0);
}
