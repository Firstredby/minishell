/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:42:59 by aorth             #+#    #+#             */
/*   Updated: 2025/06/27 16:21:42 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int	exe_prep(t_cmd *cmd)
{
	int		i_loop;
	char	*order;

	i_loop = 1;
	order = NULL;
	while (cmd)
	{
		cmd->node_nbr = i_loop;
		order = ft_itoa(cmd->node_nbr);
		if (!order)
			return (1);
		if (cmd->limiter)
			cmd->filename = create_filename("/tmp/heredoc", order, ".tmp");
		free(order);
		if (!cmd->filename)
			return (ft_putstr_fd("malloc error\n", 2), 1);
		handle_heredoc(cmd);
		i_loop++;
		cmd = cmd->next;
	}
	return (0);
}

void	exe_help(int status, t_cmd *cmd, pid_t pid)
{
	int	sig;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			g_exit_status = 130;
		else if (sig == SIGQUIT)
		{
			g_exit_status = 131;
			printf("Quit (core dumped)\n");
		}
	}
	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	if (cmd->fd_out > 2)
		close(cmd->fd_out);
	if (cmd->fd > 2)
		close(cmd->fd_out);
}

void	child_cleanup_and_exit(t_cmd *cmd, t_env *env, int exit_code, t_data *data, pid_t *pid)
{
	child_safe_cleanup(cmd);
	env_cleaner(env);
	if (pid)
		free(pid);
	pid = NULL;
	if (data)
		free(data);
	data = NULL;
	exit(exit_code);
}

void	exe_cmd(t_cmd *cmd, t_env **env, t_data *data)
{
	pid_t	pid;
	int		status;

	status = 0;
	builtin_parent(cmd, env);
	if (cmd->cmd && (!ft_strcmp(cmd->cmd, "exit")
			|| !ft_strcmp(cmd->cmd, "cd")
			|| (!ft_strcmp(cmd->cmd, "export") && !cmd->next && cmd->args[1])
			|| !ft_strcmp(cmd->cmd, "unset")))
		return ;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (handle_redir(cmd))
			child_cleanup_and_exit(cmd, *env, g_exit_status, data, NULL);
		if (cmd->cmd && is_builtin(cmd))
		{
			run_builtin(cmd, *env);
			child_cleanup_and_exit(cmd, *env, g_exit_status, data, NULL);
		}
		else
		{
			if (!*cmd->cmd)
				child_cleanup_and_exit(cmd, *env, 0, data, NULL);
			if (execvp(cmd->cmd, cmd->args) == -1)
			{
				undef_cmd(cmd->cmd);
				child_cleanup_and_exit(cmd, *env, g_exit_status, data, NULL);
			}
		}
	}
	else if (pid > 0)
		exe_help(status, cmd, pid);
	else
		perror("Fork failed");
}
