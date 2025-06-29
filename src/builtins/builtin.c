/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:31:27 by aorth             #+#    #+#             */
/*   Updated: 2025/06/29 19:45:07 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>

int	is_builtin(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd->cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd->cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd->cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd->cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd->cmd, "exit"))
		return (1);
	if (!ft_strcmp(cmd->cmd, "unset"))
		return (1);
	return (0);
}

void	run_builtin(t_cmd *cmd, t_env *env)
{
	int	i;

	if (!ft_strcmp(cmd->cmd, "echo"))
		ft_echo(cmd);
	if (!ft_strcmp(cmd->cmd, "pwd"))
		ft_pwd(cmd);
	if (!ft_strcmp(cmd->cmd, "env"))
		ft_env(cmd, env);
	if (!ft_strcmp(cmd->cmd, "export") && !cmd->args[1])
	{
		ft_export(env);
		i = 0;
		while (env->exported_envs[i])
			printf("declare -x %s\n", env->exported_envs[i++]);
	}
	return ;
}

void	builtin_parent(t_cmd *cmd, t_env **env)
{
	int	i;

	if (!cmd->cmd)
		return ;
	if (!ft_strcmp(cmd->cmd, "export") && !cmd->next && cmd->args[1])
	{
		i = 1;
		while (cmd->args[i])
			ft_export_add(cmd, env, i++);
	}
	if (!ft_strcmp(cmd->cmd, "unset"))
		ft_unset(cmd, env);
	if (!ft_strcmp(cmd->cmd, "exit"))
		g_exit_status = ft_exit(cmd);
	if (!ft_strcmp(cmd->cmd, "cd"))
		ft_cd(cmd, env);
}
