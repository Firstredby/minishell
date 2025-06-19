/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:33:59 by aorth             #+#    #+#             */
/*   Updated: 2025/06/19 21:24:29 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

void    heredoc_helper(t_cmd *cmd)
{
    if (cmd->fd_in == 0 && *cmd->limiter)
    {
        cmd->fd_in = open(cmd->filename, O_RDONLY);
        if (cmd->fd_in == -1)
        {
            perror("open for read");
            return;
        }
    }
}

int    sigs_help(t_cmd *cmd, char *line, int i)
{
    if (!line)
    {
        write(STDOUT_FILENO, "\n", 1);
        close(cmd->fd);
        unlink(cmd->filename); 
        command_sigs(); 
        return(1);
    }
    if(!ft_strcmp(line, cmd->limiter[i]))
        return(2);
    return (0);
}

void    handle_heredoc(t_cmd *cmd)
{
    char *line;
    int i;

    i=0;
    while(cmd->limiter && cmd->limiter[i] && *cmd->limiter[i])
    {
            cmd->fd = open( cmd->filename, O_WRONLY | O_CREAT | O_TRUNC , 0644);
            if (cmd->fd != -1)
            {
                heredoc_sigs();
                while(1)
                {
                    line = readline("> ");
                    if (sigs_help(cmd, line, i) == 1)
                        return;
                    else if (sigs_help(cmd, line, i) == 2)
                        break;
                    //if (!line || !ft_strcmp(line, cmd->limiter[i]))
                    //    break;
                    write(cmd->fd, line, ft_strlen(line));
                    write(cmd->fd, "\n", 1);
                    free(line);
                }
                command_sigs();
            }
            close(cmd->fd);
            i++;
    }
    heredoc_helper(cmd);
}

// void    handle_heredocv2(t_cmd *cmd)
// {
//     char *line;

//     if (cmd->limiter)
//     {
//         cmd->fd_in = open(create_filename("/tmp/heredoc",ft_itoa(cmd->node_nbr), ".tmp"), O_WRONLY | O_CREAT | O_TRUNC , 0644);
//         if (cmd->fd_in != -1)
//         {
//             while(1)
//             {
//                 line = readline("> ");
//                 if (!line || !ft_strcmp(line, cmd->limiter))
//                     break;
//                 write(cmd->fd_in, line, ft_strlen(line));
//                 write(cmd->fd_in, "\n", 1);
//                 free(line);
//             }
//         }
//         close(cmd->fd_in);    
//         // cmd->fd_in = open("/tmp/heredoc.tmp", O_RDONLY);
//         // if (cmd->fd_in == -1)
//         // {
//         //     perror("open for read");
//         //     return;
//         // }
//         // //dup2(cmd->fd_in, STDIN_FILENO);
//         // close(cmd->fd_in);
//     }
// }



void    handle_redirV2(t_cmd *cmd)
{
    // char *line;
    
    // if (cmd->limiter)
    // {
    //     cmd->fd = open("/tmp/heredoc.tmp", O_WRONLY | O_CREAT | O_TRUNC , 0644);
    //     if (cmd->fd != -1)
    //     {
    //         while(1)
    //         {
    //             line = readline("> ");
    //             if (!line || !ft_strcmp(line, cmd->limiter))
    //                 break;
    //             write(cmd->fd, line, ft_strlen(line));
    //             write(cmd->fd, "\n", 1);
    //             free(line);
    //         }
    //     }
    //     //dup2(cmd->fd, 0);
    //     close(cmd->fd);    
    //     cmd->fd = open("/tmp/heredoc.tmp", O_RDONLY);
    //     if (cmd->fd == -1)
    //     {
    //         perror("open for read");
    //         return;
    //     }
    //     dup2(cmd->fd, STDIN_FILENO);
    //     close(cmd->fd);
    // }
    // // dup2(cmd->fd, 0);
    // close (cmd->fd);
    if (cmd->fd_out == -1 || cmd->fd_in == -1)
		exit(g_exit_status);
    if (cmd->fd_out != 0)
    {
        dup2(cmd->fd_out, STDOUT_FILENO);
        close(cmd->fd_out);
    }
    if (cmd->fd_in != 0)
    {
        dup2(cmd->fd_in, STDIN_FILENO);
        close(cmd->fd_in);
        //close(cmd->fd);
    }
}
