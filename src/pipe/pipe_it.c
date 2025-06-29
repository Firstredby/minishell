/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_it.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:43:20 by aorth             #+#    #+#             */
/*   Updated: 2025/06/30 01:05:52 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>
#include <sys/types.h>

static int	alloc_pipe(t_cmd *cmd)
{
	t_cmd	*current;
	int		i;

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
	i = 0;
	while (i < cmd->pipe->pipe_count)
	{
		cmd->pipe->fds[i] = malloc(sizeof(int) * 2);
		if (cmd->pipe->fds[i] == NULL)
		{
			while (--i >= 0)
				free(cmd->pipe->fds[i]);
			free(cmd->pipe->fds);
			cmd->pipe->fds = NULL;
			return (perror("malloc"), -1);
		}
		if (pipe(cmd->pipe->fds[i]) == -1)
		{
			while (i >= 0)
				free(cmd->pipe->fds[i--]);
			free(cmd->pipe->fds);
			cmd->pipe->fds = NULL;
			return (perror("pipe"), -1);
		}
		i++;
	}
	return (0);
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

void	pipe_exit_status(int status)
{
	int	sig;

	sig = 0;
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
}

static void	free_pipes(t_pipe *pipe, t_cmd *cmd, pid_t *pid, int status)
{
	int	i;

	i = 0;
	while (i < pipe->pipe_count)
	{
		close(pipe->fds[i][0]);
		close(pipe->fds[i][1]);
		i++;
	}
	i = 0;
	while (i < cmd->pipe->cmd_count)
	{
		waitpid(pid[i], &status, 0);
		if (i == cmd->pipe->cmd_count - 1)
			pipe_exit_status(status);
		i++;
	}
	i = 0;
	while (i < pipe->pipe_count)
		free(pipe->fds[i++]);
	free(pipe->fds);
	pipe->fds = NULL;
	free(pid);
	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	if (cmd->fd_out > 2)
		close(cmd->fd_out);
	if (cmd->fd > 2)
		close(cmd->fd_out);
}

static void	init_tpipe(t_cmd *cmd)
{
	t_cmd	*current;
	t_pipe	*pipe;

	pipe = ft_calloc(sizeof(t_pipe), 1);
	if (!pipe)
		return (perror("malloc"));
	pipe->cmd_count = 0;
	pipe->pipe_count = 0;
	current = cmd;
	while (current)
	{
		current->pipe = pipe;
		current = current->next;
	}
}

void	skip_broken_commands(t_cmd *cmd)
{
	t_cmd	*curr;

	curr = cmd;
	while (curr)
	{
		if (curr->fd_in == -1 || curr->fd_out == -1)
			curr->skip = true;
		curr = curr->next;
	}
}

void	execute_pipe(t_cmd *cmd, t_data *data)
{
	t_cmd	*current;
	pid_t	*pid;
	int		i;
	t_pipe	*pipe;
	int		status;

	status = 0;
	skip_broken_commands(cmd);
	init_tpipe(cmd);
	if (alloc_pipe(cmd) == -1)
		return ;
	pipe = cmd->pipe;
	current = cmd;
	pid = malloc(sizeof(pid_t) * pipe->cmd_count);
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
	free_pipes(pipe, cmd, pid, status);
}
