/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:01:35 by aorth             #+#    #+#             */
/*   Updated: 2025/06/19 15:14:08 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_unset(t_cmd *cmd, t_env *env)
{
   // t_env *env_test = env;
    t_env *env_temp;
    t_env *env_loop;

    env_temp = NULL;
    //env = env;
    env_loop = env;
   // printf("TEST3\n");
    if(cmd && !cmd->next)
    {
      //  printf("TEST4\n");
        while (env_loop)
        {
            //printf("TEST5\n");
            if (env_loop->next && cmd->args[1] && ft_strcmp(cmd->args[1], env_loop->next->key) == 0)
            {
               // printf("TEST6\n");
            // printf("BOTH: %s\n KEY: %s\n VALUE: %s\n",env_loop->next->both, env_loop->next->key, env_loop->next->value);
                free(env_loop->next->both);
                free(env_loop->next->key);
                free(env_loop->next->value);
                env_temp = env_loop->next;
                env_loop->next = env_loop->next->next;
            // printf("BOTH: %s\n KEY: %s\n VALUE: %s\n",env_loop->next->both, env_loop->next->key, env_loop->next->value);
                free(env_temp);
                env_temp = NULL;
                //return(0);
            }
            env_loop = env_loop->next;
        }
    }
    // while (env_test)
    // {
    //     printf("Test %s\n", env_test->both);
    //     env_test = env_test->next;
    // }
    //printf("TEST7\n");
    
    return (0);
}
