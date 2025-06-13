/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:01:35 by aorth             #+#    #+#             */
/*   Updated: 2025/06/11 12:39:16 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_unset(t_cmd *cmd, t_env *env)
{
   // t_env *env_test = env;
    t_env *env_temp;

    env_temp = NULL;
    //env = env;
    while (env)
    {
        printf("KEY: %s\n", env->next->key);
        if (ft_strcmp(cmd->args[1], env->next->key) == 0)
        {
            printf("BOTH: %s\n KEY: %s\n VALUE: %s\n",env->next->both, env->next->key, env->next->value);
            free(env->next->both);
            free(env->next->key);
            free(env->next->value);
            env_temp = env->next;
            env->next = env->next->next;
            printf("BOTH: %s\n KEY: %s\n VALUE: %s\n",env->next->both, env->next->key, env->next->value);
            free(env_temp);
            env_temp = NULL;
            //return(0);
        }
        env = env->next;
    }
    // while (env_test)
    // {
    //     printf("Test %s\n", env_test->both);
    //     env_test = env_test->next;
    // }
    
    return (0);
}
