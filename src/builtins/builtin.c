/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:31:27 by aorth             #+#    #+#             */
/*   Updated: 2025/06/13 21:54:08 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int     is_builtin(t_cmd *cmd)
{
    if (!ft_strcmp(cmd->cmd, "echo") /* || !ft_strcmp(cmd->cmd, "echo ") */)
        return (1);
    if (!ft_strcmp(cmd->cmd, "pwd") /* || !ft_strcmp(cmd->cmd, "pwd ") */)
        return (1);
    if (!ft_strcmp(cmd->cmd, "cd")/*  || !ft_strcmp(cmd->cmd, "cd ") */)
        return (1);
    if (!ft_strcmp(cmd->cmd, "env") /* || !ft_strcmp(cmd->cmd, "env ") */)
        return (1);
    if (!ft_strcmp(cmd->cmd, "export") /* || !ft_strcmp(cmd->cmd, "export ") */)
        return (1);
    if (!ft_strcmp(cmd->cmd, "exit"))
        return (1);
    if (!ft_strcmp(cmd->cmd, "unset"))
        return (1);    
    return (0);
}

void    run_builtin(t_cmd *cmd, t_env *env)
{
    if (!ft_strcmp(cmd->cmd, "echo")/*  || !ft_strcmp(cmd->cmd, "echo ") */)
        ft_echo(cmd);
    if (!ft_strcmp(cmd->cmd, "pwd")/*  || !ft_strcmp(cmd->cmd, "pwd ") */)
        ft_pwd(cmd);
    if (!ft_strcmp(cmd->cmd, "cd")/*  || !ft_strcmp(cmd->cmd, "cd ") */)
        ft_cd(cmd);
    if (!ft_strcmp(cmd->cmd, "env"))
        ft_env(cmd, env);
    if (!ft_strcmp(cmd->cmd, "export"))
        ft_export(cmd, env);
    if (!ft_strcmp(cmd->cmd, "unset"))
        ft_unset(cmd, env);
    return;
    // if (!ft_strcmp(cmd->cmd, "exit"))
	// {
	// 	exit(ft_exit(cmd));
	// 	printf("its exits\n"); // exits only child process
	// 	//please exit not whole programm
	// }
} 
