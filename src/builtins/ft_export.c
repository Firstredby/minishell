/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:02:34 by aorth             #+#    #+#             */
/*   Updated: 2025/06/11 11:28:02 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <sys/types.h>

static void     alloc_export(t_env *env, int count)
{
    int i;
    t_env *env_loop;

    env->exported_envs = ft_calloc(sizeof(char *), (count + 1));
    if (!env->exported_envs)
        return(perror("malloc:"));
    i = 0;
    env_loop = env;
    while (env_loop)
    {
        printf("ISTHERERER__???%s\n", env->exported_envs[i]);
        env->exported_envs[i] = ft_strdup(env_loop->both);
        if (!env->exported_envs[i])
        {
            while (i >= 0)
            {
                free(env->exported_envs[i]);
                i--;
            }
            free(env->exported_envs);
            return(perror("malloc:"));
            
        }
        env_loop = env_loop->next;
        i++;
    }
}

static void    sort_export(t_env *env, int count)
{
    int i;
    int j;
    char *temp;

    i = 0;
    while (i < count - 1)
    {
        j = 0;
        while (j < count - i - 1)
        {
            if (ft_strcmp(env->exported_envs[j], env->exported_envs[j + 1]) > 0)
            {
                temp = env->exported_envs[j];
                env->exported_envs[j] = env->exported_envs[j + 1];
                env->exported_envs[j + 1] = temp;
            }
            j++;
        }
        i++;
    }
}

void    add_export(t_cmd *cmd, t_env *env)
{
    env_add(&env, cmd->args[1]);
}


int ft_export(t_cmd *cmd, t_env *env) // have to save the sorted envp in another variable
{
    int i;
    int count;
    t_env *env_loop;

    if(cmd->args[0] && !cmd->args[1])
    {
        env->exported_envs = NULL;
        printf("TEST\n");
        count = 0;
        env_loop = env;
        while (env_loop)
        {
            count++;
            env_loop = env_loop->next;
        }
        alloc_export(env, count);
        sort_export(env, count);
        i = 0;
        while (env->exported_envs[i])
        {
            printf("declare -x %s\n", env->exported_envs[i]);
            printf("INDEX:%d\n", i);
            //free(env->exported_envs[i]);
            i++;
        }
        //free(env->exported_envs);
        //env->exported_envs = NULL;
    }
    else {
        printf("WHAT\n");
        add_export(cmd, env);
    }
    return (0);
}









// int ft_export(t_cmd *cmd, t_env *env)
// {
    // env->exported_envs = malloc(sizeof(char *) * (count + 1));
    // if (!env->exported_envs)
    //     return (1);
    // while (env->envp[i])
    // {
    //     env->exported_envs[i] = ft_strdup(env->envp[i]);
    //     if (!env->exported_envs[i])
    //     {
    //         while (i >= 0)
    //         {
    //             free(env->exported_envs[i]);
    //             i--;
    //         }
    //         free(env->exported_envs);
    //         return (1);
    //     }
    //     i++;
    // }
    // i = 0;
    // while (i < count - 1)
    // {
    //     j = 0;
    //     while (j < count - i - 1)
    //     {
    //         if (ft_strcmp(env->exported_envs[j], env->exported_envs[j + 1]) > 0)
    //         {
    //             temp = env->exported_envs[j];
    //             env->exported_envs[j] = env->exported_envs[j + 1];
    //             env->exported_envs[j + 1] = temp;
    //         }
    //         j++;
    //     }
    //     i++;
    // }
// }