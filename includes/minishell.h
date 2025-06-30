/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 10:25:57 by aorth             #+#    #+#             */
/*   Updated: 2025/06/30 02:09:10 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern int		g_exit_status;
// tokenizer_utils
int				ft_ismetachr(int c);
t_token_type	token_type(char *type);
t_token			*newtkn(char *token, t_token_type type);
void			addtkn(t_token **list, t_token *node);
void			set_space(char **input, t_token **list);

// tokenizer
int				parser_validator(t_token **token);
t_token			**tokenizer(char *input, size_t size);

// parser_utils
char			*replace_string(char *str, char *var, int i, int k);
char			*env_from_list(t_env *env, char *key);
void			open_fd(t_token *token, t_cmd *cmd, int redir, bool *flag);
bool			is_redir(t_token_type type);
char			*dquote_expansion(t_token *token, t_env *env);
void			collect_limiter(t_token *token, t_cmd *cmd, int index);

// parser_counters
size_t			command_count(char *input);
int				lim_size(t_token *token);
int				cmd_size(t_token *token);

// parser
t_cmd			*parser(t_token **tokens, t_env *env);

// cleaner
void			trash_collector_goes_brrrr(t_token **list);
void			cmd_cleaner(t_cmd *cmd);
void			env_cleaner(t_env *env);
void			child_safe_cleanup(t_cmd *cmd);
void			pipe_cleaner(t_pipe *pipe);
void			child_cleanup_and_exit(int exit_code, t_data *data, pid_t *pid);

// cleaner utils
void			free_all(t_cmd *cmd, t_env *env, t_token **token);
void			free2d(char **list);
void			closefd(int fd);

// error messages
void			syn_err(t_token *token);
void			miss_quote(void);
void			undef_cmd(char *str);
void			file_not_exists(char *str);

// builtins
int				ft_echo(t_cmd *cmd);
int				ft_pwd(t_cmd *cmd);
int				ft_cd(t_cmd *cmd, t_env **env);
int				cd_check(t_cmd *cmd, t_env *env);
int				change_dir(t_env *env_loop, char *value);
int				take_pwd(t_env *env, char **temp);
int				ft_exit(t_cmd *cmd);
int				is_builtin(t_cmd *cmd);
int				ft_env(t_cmd *cmd, t_env *env);
int				ft_export(t_env *env);
int				ft_unset(t_cmd *cmd, t_env **env);
void			run_builtin(t_cmd *cmd, t_env *env);
int				builtin_parent(t_cmd *cmd, t_env **env);

// export
void			no_env_export(t_env **env, t_cmd *cmd, int index);
int				ft_export_add(t_cmd *cmd, t_env **env, int index);
int				export_add_help(t_cmd *cmd, int index);
int				export_check(t_cmd *cmd, t_env *env, int index);
int				alloc_export(t_env *env, int count);
char			*set_exported_env(t_env *env);

// env
int				env_add(t_env **env_head, char *env);
t_env			*env_create(char *env);
char			*env_strdup(char *env, bool flag);

// execution
void			exe_cmd(t_cmd *cmd, t_env **env, t_data *data);
void			execute_pipe(t_cmd *cmd, t_data *data);
int				handle_redir(t_cmd *cmd);
void			run_notbuiltin(t_cmd *cmd, t_env **env, t_data *data,
					pid_t *pod);
void			skip_broken_commands(t_cmd *cmd);
void			init_tpipe(t_cmd *cmd);
void			pipe_exit_status(int status);
int				alloc_pipe_help(t_cmd *cmd);

// execution preparation
int				handle_heredoc(t_cmd *cmd);
char			*create_filename(char *path, char *id, char *type);
int				exe_prep(t_cmd *cmd);

// utils
void			*ft_calloc(size_t nmemb, size_t size);
char			*ft_strdup(const char *s);
char			*ft_substr(char const *s, unsigned int start, size_t len);
void			ft_memcpy(void *dest, const void *src, size_t n);
long long		ft_atoi(const char *str);
int				ft_isdigit(int c);
int				ft_isalpha(int c);
int				ft_isalnum(int c);
void			ft_putstr_fd(char *s, int fd);
size_t			ft_strlen(const char *s);
int				ft_strcmp(const char *s1, const char *s2);
char			**ft_split(char const *s, char c);
int				env_handle(char **env, t_env **env_head);
int				env_strcmp(const char *s1, const char *s2);
size_t			command_count(char *input);
void			ft_strjoin_free(char **s1, char *s2);
char			*ft_itoa(int n);
int				ft_strchr(const char *s, int c);
int				ft_isalpha(int c);
void			ft_strjoin_free_cursed(char **s1, char *s2);
char			*ft_strstr(const char *haystack, const char *needle);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				looking_path(t_cmd *cmd, char **env);
int				reality_check(char *input);

// debug panel
void			show_args(t_cmd *cmd);
void			show_token(t_token **token);
void			show_env(t_env *env);

// signal_handler
void			main_sigs(void);
void			command_sigs(void);
void			handle_sigs(int sig);
void			heredoc_sigs(void);
