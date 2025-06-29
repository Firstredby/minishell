/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 00:45:30 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/30 00:46:10 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*replace_string(char *str, char *var, int i, int k)
{
	char	*newstr;
	int		size;
	int		str_pos;
	int		var_pos;

	str_pos = 0;
	var_pos = 0;
	size = i - 1 + ft_strlen(var) + (ft_strlen(str) - i - k + 1);
	newstr = ft_calloc(size, sizeof(char));
	if (!newstr)
		return (NULL);
	while (str_pos < i - 1)
	{
		newstr[str_pos] = str[str_pos];
		str_pos++;
	}
	while (var && var[var_pos])
		newstr[str_pos++] = var[var_pos++];
	while (str[i + k])
		newstr[str_pos++] = str[i + k++];
	free(str);
	free(var);
	return (str = newstr);
}

char	*env_from_list(t_env *env, char *key)
{
	t_env	*head;

	head = env;
	if (!ft_strcmp(key, "?"))
	{
		free(key);
		key = ft_itoa(g_exit_status);
		return (key);
	}
	while (head)
	{
		if (!ft_strcmp(head->key, key))
		{
			free(key);
			key = ft_strdup(head->value);
			return (key);
		}
		head = head->next;
	}
	free(key);
	key = ft_calloc(1, 1);
	return (key);
}

void	open_fd(t_token *token, t_cmd *cmd, int redir, bool *flag)
{
	int	flags;
	int	*fd;

	if (redir == T_RED_IN)
	{
		flags = O_RDONLY;
		fd = &cmd->fd_in;
	}
	else
		fd = &cmd->fd_out;
	if (redir == T_RED_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (redir == T_RED_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	if (*fd > 2)
		close(*fd);
	*fd = 0;
	*fd = open(token->next->token, flags, 0644);
	if (*fd == -1)
		(file_not_exists(token->next->token), *flag = false);
}

void	collect_limiter(t_token *token, t_cmd *cmd, int index)
{
	if (token->next->type == T_DOLLAR)
	{
		cmd->limiter[index] = ft_strdup("$");
		ft_strjoin_free(&cmd->limiter[index++], token->next->token);
	}
	else
		cmd->limiter[index] = ft_strdup(token->next->token);
	if (!cmd->limiter[index])
		return ;
	if (cmd->fd_in)
		close(cmd->fd_in);
	cmd->fd_in = 0;
}

char	*dquote_expansion(t_token *token, t_env *env)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (token->token[i])
	{
		if (token->token[i++] == '$')
		{
			while (!ft_ismetachr(token->token[i + k]) && token->token[i
					+ k] != '|' && token->token[i + k] != '/')
				k++;
			if (k == 0)
				continue ;
			else
				token->token = replace_string(token->token, env_from_list(env,
							ft_substr(token->token, i, k)), i, k);
			if (!token->token)
				return (NULL);
			break ;
		}
	}
	if (ft_strlen(token->token) != (size_t)i)
		return (dquote_expansion(token, env));
	return (token->token);
}
