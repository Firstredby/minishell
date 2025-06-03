/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 10:25:59 by aorth             #+#    #+#             */
/*   Updated: 2025/05/25 14:44:38 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    int				fd;
    int				fd_in;
    int				fd_out;
	char			*limiter;
    t_env			*env;
	struct s_cmd	*next;
}   t_cmd;

typedef enum e_token_type {
    T_WORD,
    T_PIPE,
    T_RED_IN,
    T_RED_OUT,
    T_RED_APPEND,
    T_HEREDOC,
    T_EOF,
	T_DOLLAR,
	T_RED_TARGET,
    T_QUOTE
}   t_token_type;

typedef struct s_token
{
	char			*token;
	t_token_type	type;
	struct s_token	*next;
}	t_token;
