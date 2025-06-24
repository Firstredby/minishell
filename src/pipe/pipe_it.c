/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_it.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:43:20 by aorth             #+#    #+#             */
/*   Updated: 2025/06/24 17:42:50 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>
#include <sys/types.h>

static int    alloc_pipe(t_cmd *cmd)
{
    t_cmd *current;
    int i;

    current = cmd;
    while (current)
    {
        current = current->next;
        cmd->pipe->cmd_count++;
    }
    cmd->pipe->pipe_count = cmd->pipe->cmd_count - 1; 
    cmd->pipe->fds = malloc(sizeof(int*) * cmd->pipe->pipe_count);
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

static void    child_cleanup_and_exit(t_cmd *cmd, t_env *env, int exit_code)
{
    child_safe_cleanup(cmd);
    env_cleaner(env);
    exit(exit_code);
}

static void    assign_fds(int i, t_cmd *cmd, t_pipe *pipe, t_env *env)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    if (i == 0)
        dup2(pipe->fds[0][1], STDOUT_FILENO);
    else if (i == pipe->cmd_count - 1)
        dup2(pipe->fds[i-1][0], STDIN_FILENO);
    else 
    {
        dup2(pipe->fds[i-1][0], STDIN_FILENO);
        dup2(pipe->fds[i][1], STDOUT_FILENO);
    }
    int j = 0;
    while (j < pipe->pipe_count)
    {
        close(pipe->fds[j][0]);
        close(pipe->fds[j][1]);
        j++;
    }
    handle_redirV2(cmd);
    if (is_builtin(cmd))
    {
        run_builtin(cmd, env);
        child_cleanup_and_exit(cmd, env, g_exit_status);
    }
    else
    {
        if (!cmd->cmd)
        {
            ft_putstr_fd("minishell: command not found\n", STDERR_FILENO);
            child_cleanup_and_exit(cmd, env, 127);
        }
        if(execvp(cmd->cmd, cmd->args) == -1)
        {
            undef_cmd(cmd->cmd);
            child_cleanup_and_exit(cmd, env, g_exit_status);            
        }

    }
    child_cleanup_and_exit(cmd, env, 0);
}

void   pipe_exit_status(int status)
{
    int sig;

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

static void    free_pipes(t_pipe *pipe, t_cmd *cmd, pid_t *pid, int status)
{
    int i;

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
    {
        free(pipe->fds[i]);
        i++;
    }
    free(pipe->fds);
    pipe->fds = NULL;
    free(pid);
    if (cmd->fd_in > 2) close(cmd->fd_in);
    if (cmd->fd_out > 2) close(cmd->fd_out);
    if (cmd->fd > 2) close(cmd->fd_out);
}


// void execute_pipe2(t_cmd *cmd, t_env *env)
// {
//     t_cmd *current;
//     t_pipe *pipe;
//     pid_t *pid;
//     int i;

//     pipe = ft_calloc(sizeof(t_pipe), 1);
//     if (!pipe)
//         return(perror("malloc"));
//     cmd->pipe = pipe;
//     alloc_pipe(cmd);
//     current = cmd;
//     pid = malloc(sizeof(pid_t) * pipe->cmd_count);
//     if (pid == NULL)
//         return(perror("malloc"));
//     i = 0;
//     while (current)
//     {
//         printf("TEST0\n");
//         pid[i] = fork();
//         if (pid[i] == 0)
//             assign_fds(i, current, pipe, env);
//         current = current->next;
//         i++;
//     }
//     free_pipes(pipe, cmd, pid);
//     printf("asdasdasdas\n");
//     printf("Pipe executed successfully\n");
// }


static void    init_tpipe(t_cmd *cmd)
{
    t_cmd *current;
    t_pipe *pipe;

    pipe = ft_calloc(sizeof(t_pipe), 1);
    if (!pipe)
    {
        perror("malloc");
        return;
    }
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
	t_cmd *curr;

	curr = cmd;
	while (curr)
	{
		if (curr->fd_in == -1 || curr->fd_out == -1)
			curr->skip = true;
		curr = curr->next;
	}
}

void execute_pipe(t_cmd *cmd, t_env *env)
{
    t_cmd *current;
    pid_t *pid;
    int i;
    t_pipe *pipe;
    int status;

    status = 0;
	skip_broken_commands(cmd);
    init_tpipe(cmd);
    if (alloc_pipe(cmd) == -1)
        return;
    pipe = cmd->pipe;
    current = cmd;
    pid = malloc(sizeof(pid_t) * pipe->cmd_count);
    if (pid == NULL)
    {
        perror("malloc");
        return;
    }
    i = 0;
    while (current)
    {
        pid[i] = fork();
        if (pid[i] == 0)
            assign_fds(i, current, pipe, env);
        current = current->next;
        i++;
    }
    free_pipes(pipe, cmd, pid, status);
}



// void execute_pipe2(t_cmd *cmd, t_env *env)
// {
//     t_cmd *current = cmd;
//     int cmd_count = 0;
//     int pipe_count;
//     int **fds;
//     pid_t *pid;
//     int i = 0;

//     while (current)
//     {
//         current = current->next;
//         cmd_count++;
//     }
//     pipe_count = cmd_count - 1; 
//     fds = malloc(sizeof(int*) * pipe_count);
//     if (fds == NULL)
//         return(perror("malloc"));
//     i = 0;
//     while (i < pipe_count)
//     {
//         fds[i] = malloc(sizeof(int) * 2);
//         if (fds[i] == NULL)
//             return(perror("malloc"));
//         if (pipe(fds[i]) == -1)
//             return(perror("pipe"));
//         i++;
//     }
//     current = cmd;
//     pid = malloc(sizeof(pid_t) * cmd_count);
//     if (pid == NULL)
//         return(perror("malloc"));
//     i = 0;
//     while (current)
//     {
//         pid[i] = fork();
//         if (pid[i] == 0)
//         {
//             if (i == 0) 
//                 dup2(fds[0][1], STDOUT_FILENO);
//             else if (i == cmd_count - 1) 
//                 dup2(fds[i-1][0], STDIN_FILENO);
//             else 
//             {
//                 dup2(fds[i-1][0], STDIN_FILENO);
//                 dup2(fds[i][1], STDOUT_FILENO);
//             }
//             int j = 0;
//             while (j < pipe_count)
//             {
//                 close(fds[j][0]);
//                 close(fds[j][1]);
//                 j++;
//             }
//             handle_redirV2(current);
//             if (is_builtin(current))
//             {
//                 run_builtin(current, env);
//                 exit(0); 
//             }
//             else
//             {
//                 execvp(current->cmd, current->args);
//                 perror("execvp");
//                 exit(1);
//             }
//         }
//         current = current->next;
//         i++;
//     }
//     i = 0;
//     while (i < pipe_count)
//     {
//         close(fds[i][0]);
//         close(fds[i][1]);
//         i++;
//     }
    
//     i = 0;
//     while (i < cmd_count)
//     {
//         waitpid(pid[i], NULL, 0);
//         i++;
//     }
//     i = 0;
//     while (i < pipe_count)
//     {
//         free(fds[i]);
//         i++;
//     }
//     printf("Pipe executed successfully\n");
//     free(fds);
//     free(pid);
//     if (cmd->fd_in > 2) close(cmd->fd_in);
//     if (cmd->fd_out > 2) close(cmd->fd_out);
//     if (cmd->fd > 2) close(cmd->fd_out);
// }
