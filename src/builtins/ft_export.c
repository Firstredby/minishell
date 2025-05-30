/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:02:34 by aorth             #+#    #+#             */
/*   Updated: 2025/05/22 18:02:42 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_export(t_cmd *cmd) // have to save the sorted envp in another variable
{
    char *temp;
    int i = 0;
    int j;
    int count;

    if (!cmd || !cmd->env || !cmd->env->envp)
        return (1);
    count = 0;
    while (cmd->env->envp[count])
        count++;
    cmd->env->exported_envs = malloc(sizeof(char *) * (count + 1));
    if (!cmd->env->exported_envs)
        return (1);
    while (cmd->env->envp[i])
    {
        cmd->env->exported_envs[i] = ft_strdup(cmd->env->envp[i]);
        if (!cmd->env->exported_envs[i])
        {
            while (i >= 0)
            {
                free(cmd->env->exported_envs[i]);
                i--;
            }
            free(cmd->env->exported_envs);
            return (1);
        }
        i++;
    }
    i = 0;
    while (i < count - 1)
    {
        j = 0;
        while (j < count - i - 1)
        {
            if (ft_strcmp(cmd->env->exported_envs[j], cmd->env->exported_envs[j + 1]) > 0)
            {
                temp = cmd->env->exported_envs[j];
                cmd->env->exported_envs[j] = cmd->env->exported_envs[j + 1];
                cmd->env->exported_envs[j + 1] = temp;
            }
            j++;
        }
        i++;
    }
    i = 0;
    while (cmd->env->exported_envs[i])
    {
        printf("%s\n", cmd->env->exported_envs[i]);
        i++;
    }
    return (0);
}
