/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:02:34 by aorth             #+#    #+#             */
/*   Updated: 2025/06/19 20:47:26 by aorth            ###   ########.fr       */
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

// void    add_export(t_cmd *cmd, t_env *env)
// {
//     env_add(&env, cmd->args[1]);
// }

int export_add_help(t_cmd *cmd)
{
    int i;
    i=0;
    while(cmd->args[1][i] != '=' && !cmd->args[1][i])
    {
        if (!ft_isalpha(cmd->args[1][i]))
        {
            if(cmd->args[1][i] == '_')
                continue;
            else
                return (1);
        }
        i++;
    }
    return(0);
}

int ft_export_add(t_cmd *cmd, t_env *env)
{
    t_env *env_loop;
    char *temp;

    if (ft_strchr(cmd->args[1], '=') > 1 || cmd->args[1][0] == '='|| export_add_help(cmd))
    {
        ft_putstr_fd("export: `", 2);
        ft_putstr_fd(cmd->args[1], 2);
        ft_putstr_fd("': not a valid identifier\n", 2);
        g_exit_status = 1;
        exit(g_exit_status);
    }
    // env_loop = env;
    // int count = 0;
    // while (env_loop)
    // {
    //     count++;
    //     env_loop = env_loop->next;
    // }
    if (ft_strchr(cmd->args[1], '=') == 0)
    {
        // printf("args: %s\n", cmd->args[1]);
        // free(env->exported_envs);
        // alloc_export(env, count + 1);
        // env->exported_envs[count] = ft_strdup(cmd->args[1]);
        // printf("exported: %s\n", env->exported_envs[count]);
        // printf("exported1: %s\n", env->exported_envs[count-1]);
        // sort_export(env, count +1);
        printf("TEST9\n");
        env_add(&env, cmd->args[1]);
        printf("TEST10\n");
        return(0);
    }
    env_loop = env;
    temp = env_strdup(cmd->args[1], true);
    while(env_loop)
    {
        if(!ft_strcmp(temp, env_loop->key))
        {
            free(temp);
            free(env_loop->value);
            free(env_loop->both);
            env_loop->both = ft_strdup(cmd->args[1]);
            temp = env_strdup(cmd->args[1], false);
            env_loop->value = ft_strdup(temp);
            free(temp);
            return(g_exit_status = 0);
        }
        env_loop = env_loop->next;
    }
    if(!cmd->next)
    {
        printf("TEST9\n");
        env_add(&env, cmd->args[1]);
        printf("TEST10\n");
    }
    return (g_exit_status = 0);
}


int ft_export(t_cmd *cmd, t_env *env) // have to save the sorted envp in another variable
{
    int i;
    int count;
    t_env *env_loop;

    if(cmd->args[0] && !cmd->args[1])
    {
        //env->exported_envs = NULL;
        count = 0;
        env_loop = env;
        while (env_loop)
        {
            count++;
            env_loop = env_loop->next;
        }
        if (!env->exported_envs)
        {
            alloc_export(env, count);
            sort_export(env, count);
        }
        i = 0;
        while (env->exported_envs[i])
            printf("declare -x %s\n", env->exported_envs[i++]);
    }
    return (0);
}

//exit 11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
// need to change atol;









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