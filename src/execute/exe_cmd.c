/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:42:59 by aorth             #+#    #+#             */
/*   Updated: 2025/06/29 23:38:34 by ishchyro         ###   ########.fr       */
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
			return (g_exit_status=12);
		if (cmd->limiter && *cmd->limiter)
		{
			cmd->filename = create_filename("/tmp/heredoc", order, ".tmp");
			free(order);
			if (!cmd->filename)
				return (ft_putstr_fd("malloc error\n", 2), 1);
			if(handle_heredoc(cmd))
				return(1);
		}
		else
			free(order);
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
	(void)cmd;
	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	if (cmd->fd_out > 2)
		close(cmd->fd_out);
	if (cmd->fd > 2)
		close(cmd->fd);
}

void	child_cleanup_and_exit(int exit_code, t_data *data, pid_t *pid)
{
	child_safe_cleanup(data->cmd);
	close(STDOUT_FILENO);
	close(STDIN_FILENO);
	close(STDERR_FILENO);
	env_cleaner(data->env);
	if (pid)
		free(pid);
	pid = NULL;
	if (data)
		free(data);
	data = NULL;
	exit(exit_code);
}

void	run_notbuiltin(t_cmd *cmd, t_env **env, t_data *data)
{
	char	*temp;

	temp = NULL;
	if (!cmd->cmd)
		child_cleanup_and_exit(0, data, NULL);
	else if (!*cmd->cmd)
		(undef_cmd(NULL),
			child_cleanup_and_exit(0, data, NULL));
	if ((*env)->both)
		ft_export(*env);
	if (looking_path(cmd, (*env)->exported_envs))
	{
		if (ft_strncmp(cmd->cmd, "/bin/", 5) && !ft_strchr(cmd->cmd,
				'/'))
			temp = ft_strdup("/bin/");
		ft_strjoin_free(&temp, cmd->cmd);
	}
	else
		temp = ft_strdup(cmd->cmd);
	if (execve(temp, cmd->args, (*env)->exported_envs) == -1)
	{
		(undef_cmd(cmd->cmd), free(temp));
		temp = NULL;
		child_cleanup_and_exit(g_exit_status, data, NULL);
	}
}

void	exe_cmd(t_cmd *cmd, t_env **env, t_data *data)
{
	pid_t	pid;
	int		status;
	char	*temp;

	status = 0;
	temp = NULL;
	builtin_parent(cmd, env);
	if (cmd->cmd && (!ft_strcmp(cmd->cmd, "exit") || !ft_strcmp(cmd->cmd, "cd")
			|| (!ft_strcmp(cmd->cmd, "export") && !cmd->next && cmd->args[1])
			|| !ft_strcmp(cmd->cmd, "unset")))
		return ;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (handle_redir(cmd))
			child_cleanup_and_exit(g_exit_status, data, NULL);
		if (cmd->cmd && is_builtin(cmd))
		{
			run_builtin(cmd, *env);
			child_cleanup_and_exit(g_exit_status, data, NULL);
		}
		else
			run_notbuiltin(cmd, env, data);
	}
	else if (pid > 0)
	{
		exe_help(status, cmd, pid);
	}
	else
		perror("Fork failed");
}
