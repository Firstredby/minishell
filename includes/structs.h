/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 10:25:59 by aorth             #+#    #+#             */
/*   Updated: 2025/06/20 18:13:39 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <stdbool.h>

extern int g_exit_status;

typedef struct s_pipe
{
    int     cmd_count;
    int     pipe_count;
    int     **fds;
    pid_t   *pid;
}   t_pipe;

typedef struct s_env
{
    char    *key;
    char    *value;
    char    *both;
    struct s_env    *next;
    char    **envp;
    char    **exported_envs;
}   t_env;

typedef struct s_cmd
{
    char			*cmd;
    char			**args;
    int				fd;//
    int				fd_in;
    int				fd_out;
    bool            skip;
	char			**limiter;
    int             node_nbr;
    char            *filename;
    t_pipe          *pipe;
	struct s_cmd	*next;
}   t_cmd;

typedef enum e_token_type {
    T_WORD = 0,
    T_PIPE = 1,
    T_RED_IN = 2,
    T_RED_OUT = 3,
    T_RED_APPEND = 4,
    T_HEREDOC = 5,
    T_EOF = 6,
	T_DOLLAR = 7,
	T_RED_TARGET = 8,
    T_SQUOTE = 9,
	T_DQUOTE = 10,
	T_SPACE = 11
}   t_token_type;

typedef struct s_token
{
    char			*token;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_data
{
    t_env   *env;
    t_cmd   *cmd;
    t_token **token;
}   t_data;
