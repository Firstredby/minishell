/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_it.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:43:20 by aorth             #+#    #+#             */
/*   Updated: 2025/06/30 02:13:44 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>
#include <sys/types.h>

static int	alloc_pipe(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		current = current->next;
		cmd->pipe->cmd_count++;
	}
	cmd->pipe->pipe_count = cmd->pipe->cmd_count - 1;
	cmd->pipe->fds = malloc(sizeof(int *) * cmd->pipe->pipe_count);
	if (cmd->pipe->fds == NULL)
		return (perror("malloc"), -1);
	alloc_pipe_help(cmd);
	return (0);
}

void	assign_fds_help(t_cmd *cmd, t_data *data, pid_t *pid)
{
	if (cmd->skip)
		child_cleanup_and_exit(g_exit_status, data, pid);
	if (handle_redir(cmd))
		child_cleanup_and_exit(g_exit_status, data, pid);
	if (is_builtin(cmd))
	{
		run_builtin(cmd, data->env);
		child_cleanup_and_exit(g_exit_status, data, pid);
	}
	else
		run_notbuiltin(cmd, &data->env, data, pid);
}

static void	assign_fds(int i, t_cmd *cmd, t_data *data, pid_t *pid)
{
	int	j;

	(signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_DFL));
	if (i == 0 && !cmd->skip)
		dup2(data->cmd->pipe->fds[0][1], STDOUT_FILENO);
	else if (i == data->cmd->pipe->cmd_count - 1)
		dup2(data->cmd->pipe->fds[i - 1][0], STDIN_FILENO);
	else if (!cmd->skip)
	{
		dup2(data->cmd->pipe->fds[i - 1][0], STDIN_FILENO);
		dup2(data->cmd->pipe->fds[i][1], STDOUT_FILENO);
	}
	j = 0;
	while (j < data->cmd->pipe->pipe_count)
	{
		close(data->cmd->pipe->fds[j][0]);
		close(data->cmd->pipe->fds[j][1]);
		j++;
	}
	assign_fds_help(cmd, data, pid);
}

static void	free_pipes(t_pipe *pipe, t_cmd *cmd, pid_t *pid, int status)
{
	int	i;

	i = 0;
	while (i < pipe->pipe_count)
		(close(pipe->fds[i][0]), close(pipe->fds[i++][1]));
	i = 0;
	while (i < cmd->pipe->cmd_count)
	{
		waitpid(pid[i], &status, 0);
		if (i == cmd->pipe->cmd_count - 1)
			pipe_exit_status(status);
		i++;
	}
	i = 0;
	free(pid);
	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	if (cmd->fd_out > 2)
		close(cmd->fd_out);
	if (cmd->fd > 2)
		close(cmd->fd_out);
}

void	execute_pipe(t_cmd *cmd, t_data *data)
{
	t_cmd	*current;
	pid_t	*pid;
	int		i;
	int		status;

	status = 0;
	skip_broken_commands(cmd);
	init_tpipe(cmd);
	if (alloc_pipe(cmd) == -1)
		return ;
	current = cmd;
	pid = malloc(sizeof(pid_t) * cmd->pipe->cmd_count);
	if (pid == NULL)
		return (perror("malloc"));
	i = 0;
	while (current)
	{
		pid[i] = fork();
		if (pid[i] == 0)
			assign_fds(i, current, data, pid);
		current = current->next;
		i++;
	}
	free_pipes(cmd->pipe, cmd, pid, status);
}
