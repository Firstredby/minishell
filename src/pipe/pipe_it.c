/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_it.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:43:20 by aorth             #+#    #+#             */
/*   Updated: 2025/06/09 13:19:28 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>
#include <sys/types.h>

void execute_pipe(t_cmd *cmd1, t_cmd *cmd2)
{
    int fd[2];
    int fd2[2];
    pid_t pid1, pid2;
    int status;
    
    if (pipe(fd) == -1)
    {
        perror("pipe");
        return;
    }
    if (pipe(fd2) == -1)
    {
        perror("pipe");
        close(fd[0]);
        close(fd[1]);
        return;
    }
    
    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        return;
    }
    
    if (pid1 == 0) 
    {
        if (cmd1->fd != 1)
        {
            dup2(fd2[0], STDIN_FILENO);
            close(fd2[0]);
            close(fd2[1]);
        }
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);

        handle_redir(cmd1);
        if (is_builtin(cmd1))
            run_builtin(cmd1);
        else
            execvp(cmd1->cmd, cmd1->args);

        exit(EXIT_FAILURE);
    }

    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        waitpid(pid1, NULL, 0);
        return;
    }
    
    if (pid2 == 0) 
    {
        if(cmd2->next != NULL)
        {
            dup2(fd2[1], STDOUT_FILENO);
            close(fd2[0]);
            close(fd2[1]);
        }
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        
        handle_redir(cmd2);
        
        if (is_builtin(cmd2))
            run_builtin(cmd2);
        else
            execvp(cmd2->cmd, cmd2->args);
        
        exit(EXIT_FAILURE);
    }
    
    close(fd[0]);
    close(fd[1]);
    
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    printf("Pipe executed successfully\n");
}

static void    alloc_pipe(t_cmd *cmd)
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
        return(perror("malloc"));
    i = 0;
    while (i < cmd->pipe->pipe_count)
    {
        cmd->pipe->fds[i] = malloc(sizeof(int) * 2);
        if (cmd->pipe->fds[i] == NULL)
            return(perror("malloc"));
        if (pipe(cmd->pipe->fds[i]) == -1)
            return(perror("pipe"));
        i++;
    }
}

static void    assign_fds(int i, t_cmd *cmd, t_pipe *pipe)
{
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
        run_builtin(cmd);
    else
    {
        execvp(cmd->cmd, cmd->args);
        perror("execvp");
    }
    exit(0);
}

static void    free_pipes(t_pipe *pipe, t_cmd *cmd, pid_t *pid)
{
    int i;

    i = 0;
    while (i < cmd->pipe->cmd_count)
        waitpid(pid[i++], NULL, 0);
    i = 0;
    while (i < pipe->pipe_count)
    {
        close(pipe->fds[i][0]);
        close(pipe->fds[i][1]);
        i++;
    }
    i = 0;
    while (i < pipe->pipe_count)
    {
        free(pipe->fds[i]);
        i++;
    }
    free(pipe->fds);
    free(pid);
    if (cmd->fd_in > 2) close(cmd->fd_in);
    if (cmd->fd_out > 2) close(cmd->fd_out);
    if (cmd->fd > 2) close(cmd->fd_out);
}


void execute_pipe2(t_cmd *cmd)
{
    t_cmd *current;
    t_pipe *pipe;
    pid_t *pid;
    int i;

    pipe = ft_calloc(sizeof(t_pipe), 1);
    if (!pipe)
        return(perror("malloc"));
    cmd->pipe = pipe;
    alloc_pipe(cmd);
    current = cmd;
    pid = malloc(sizeof(pid_t) * pipe->cmd_count);
    if (pid == NULL)
        return(perror("malloc"));
    i = 0;
    while (current)
    {
        pid[i] = fork();
        if (pid[i] == 0)
            assign_fds(i, current, pipe);
        current = current->next;
        i++;
    }
    free_pipes(pipe, cmd, pid);
    printf("Pipe executed successfully\n");
}



// void execute_pipe2(t_cmd *cmd)
// {
//     t_cmd *current = cmd;
//     int cmd_count = 0;
//     int pipe_count;
//     int **fds;
//     pid_t *pid;
//     int i = 0;

//     alloc_pipe(cmd);
//     // while (current)
//     // {
//     //     current = current->next;
//     //     cmd_count++;
//     // }
//     // pipe_count = cmd_count - 1; 
//     // fds = malloc(sizeof(int*) * pipe_count);
//     // if (fds == NULL)
//     //     return(perror("malloc"));
//     // i = 0;
//     // while (i < pipe_count)
//     // {
//     //     fds[i] = malloc(sizeof(int) * 2);
//     //     if (fds[i] == NULL)
//     //         return(perror("malloc"));
//     //     if (pipe(fds[i]) == -1)
//     //         return(perror("pipe"));
//     //     i++;
//     // }
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
//             handle_fds(&i, current);
//         //     if (i == 0) 
//         //         dup2(fds[0][1], STDOUT_FILENO);
//         //     else if (i == cmd_count - 1) 
//         //         dup2(fds[i-1][0], STDIN_FILENO);
//         //     else 
//         //     {
//         //         dup2(fds[i-1][0], STDIN_FILENO);
//         //         dup2(fds[i][1], STDOUT_FILENO);
//         //     }
//         //     int j = 0;
//         //     while (j < pipe_count)
//         //     {
//         //         close(fds[j][0]);
//         //         close(fds[j][1]);
//         //         j++;
//         //     }
//         //     handle_redirV2(current);
//         //     if (is_builtin(current))
//         //     {
//         //         run_builtin(current);
//         //         exit(0); 
//         //     }
//         //     else
//         //     {
//         //         execvp(current->cmd, current->args);
//         //         perror("execvp");
//         //         exit(1);
//         //     }
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
