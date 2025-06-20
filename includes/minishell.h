/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 10:25:57 by aorth             #+#    #+#             */
/*   Updated: 2025/06/20 17:31:10 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "executor.h"
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>


extern int g_exit_status;
//tokenizer_utils
int	            ft_ismetachr(int c);
t_token_type	token_type(char *type);
t_token	        *newtoken(char *token, t_token_type type);
void	        addtoken(t_token **list, t_token *node);
void			set_space(char **input, t_token **list);

//tokenizer
int		parser_validator(t_token **token);
t_token	**tokenizerV3(char *input, size_t size);

//parser_utils
char	*replace_string(char *str, char *var, int i, int k);
char    *env_from_list(t_env *env, char *key);
void 	open_fd(t_token *token, t_cmd *cmd, int redir);
bool	is_redir(t_token_type type);

//parser
t_cmd	*parserV3(t_token **tokens, t_env *env);

//cleaner
void    TRASH_COLLECTOR_GOES_BRRRR(t_token **list);
void    cmd_cleaner(t_cmd *cmd);
void    env_cleaner(t_env *env);
void    free_all(t_cmd *cmd, t_env *env, t_token **token);

//error messages
void	syn_err(t_token *token);
void	miss_quote(void);
void	undef_cmd(char *str);
void	file_not_exists(char *str);

//builtins
int		ft_echo(t_cmd *cmd);
int		ft_pwd(t_cmd *cmd);
int		ft_cd(t_cmd *cmd);
int		ft_exit(t_cmd *cmd);
int		is_builtin(t_cmd *cmd);
int		ft_env(t_cmd *cmd, t_env *env);
int		ft_export(t_cmd *cmd, t_env *env);
int		ft_unset(t_cmd *cmd, t_env *env);
void    run_builtin(t_cmd *cmd, t_env *env);
void    builtin_parent(t_cmd *cmd, t_env *env);
int     ft_export_add(t_cmd *cmd, t_env *env, int index);

//env
int		env_add(t_env **env_head, char *env);
t_env	*env_create(char *env);
char    *env_strdup(char *env, bool flag);

//execution
void	exe_cmd(t_cmd *cmd, t_env *env);
void	execute_pipe(t_cmd *cmd, t_env *env);
void    handle_redirV2(t_cmd *cmd);

//execution preparation
void    handle_heredoc(t_cmd *cmd);
char    *create_filename(char *path, char *id, char *type);
void    exe_prep(t_cmd *cmd);

//utils
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	ft_memcpy(void *dest, const void *src, size_t n);
unsigned long	ft_atoi(const char *str);
int		ft_isdigit(int c);
int     ft_isalpha(int c);
int     ft_isalnum(int c);
void	ft_putstr_fd(char *s, int fd);
size_t	ft_strlen(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
char	**ft_split(char const *s, char c);
int		env_handle(char **env, t_env **env_head);
int		env_strcmp(const char *s1, const char *s2);
size_t	command_count(char *input);
void	ft_strjoin_free(char **s1, char *s2);
char	*ft_itoa(int n);
int     ft_strchr(const	char *s, int c);
int	    ft_isalpha(int c);

//debug panel
void	show_args(t_cmd *cmd);
void	show_token(t_token **token);

//signal_handler
void    main_sigs(void);
void    command_sigs(void);
void    handle_sigs(int sig);
void    heredoc_sigs(void);
