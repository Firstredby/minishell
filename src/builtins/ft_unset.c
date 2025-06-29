/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:01:35 by aorth             #+#    #+#             */
/*   Updated: 2025/06/29 12:14:18 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_unset(t_cmd *cmd, t_env **env)
{
	t_env	*env_temp;
	t_env	*env_loop;
	int flag;
	int i;

	flag = 0;
	i = 0;
	env_temp = NULL;
	env_loop = *env;
	if (cmd && !cmd->next)
	{
        if (cmd->args[1] && env_loop && !ft_strcmp(cmd->args[1], env_loop->key))
        {
            if (env_loop->next)
            {
                env_loop->next->exported_envs = env_loop->exported_envs;
                env_loop->exported_envs = NULL; 
                *env = env_loop->next;
            }
            else
            {
                free2d(env_loop->exported_envs);
                *env = NULL;
            }
            
            free(env_loop->both);
            free(env_loop->key);
            free(env_loop->value);
            free(env_loop);
            
            return (g_exit_status = 0);
        }
		while (env_loop)
		{
			if (env_loop->next && cmd->args[1]
				&& !ft_strcmp(cmd->args[1], env_loop->next->key))
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
	return (g_exit_status = 0);
}
