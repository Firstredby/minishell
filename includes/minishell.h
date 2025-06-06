/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 10:25:57 by aorth             #+#    #+#             */
/*   Updated: 2025/05/22 21:01:50 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "executor.h"
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

t_cmd	*tokenizer(char *input, t_env *env);
int ft_echo(t_cmd *cmd);
int ft_pwd(t_cmd *cmd);
int ft_cd(t_cmd *cmd);
int ft_exit(t_cmd *cmd);
void    exe_cmd(t_cmd *cmd);
void    redir_append(t_cmd *cmd, int i);
void    redir_out(t_cmd *cmd, int i);
void    redir_in(t_cmd *cmd, int i);
void    handle_redir(t_cmd *cmd);
int     is_builtin(t_cmd *cmd);
void    run_builtin(t_cmd *cmd);
void    ft_free_split(char **split);
void    execute_pipe(t_cmd *cmd1, t_cmd *cmd2);
int ft_env(t_cmd *cmd);
int ft_export(t_cmd *cmd);
int    ft_unset(t_cmd *cmd);
void    execute_pipe2(t_cmd *cmd);
void	ll_free(t_cmd *cmd);
t_cmd	*parser(char *input, t_env *env);
t_cmd	*parserV3(char *input, t_env *env);
t_token	**tokenizerV3(char *input, size_t size);

//utils
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
void	ft_memcpy(void *dest, const void *src, size_t n);
long	ft_atoi(const char *str);
int		ft_isdigit(int c);
void	ft_putstr_fd(char *s, int fd);
size_t	ft_strlen(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
char	**ft_split(char const *s, char c);
int		env_handle(char **env, t_env **env_head);
int		env_strcmp(const char *s1, const char *s2);
void	show_token(char *input);
size_t	command_count(char *input);
void	show_args(t_cmd *cmd);