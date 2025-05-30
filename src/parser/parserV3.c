/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserV3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:09:41 by ishchyro          #+#    #+#             */
/*   Updated: 2025/05/26 15:37:27 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	command_count(char *input)
{
	int		i;
	size_t	result;

	result = 0;
	i = 0;
	while (input[i])
		if (input[i++] == '|')
			result++;
	if (!result)
		return (1);
	return (result);
}

bool	is_redir(t_token *token)
{
	if (token->type == T_RED_OUT || token->type == T_RED_IN
		|| token->type == T_HEREDOC || token->type == T_RED_APPEND)
		return (true);
	return (false);
}

int	arg_count(t_token *token)
{
	t_token	*curr;
	int	i;

	curr = token;
	i = 0;
	while (curr && curr->type != T_PIPE)
	{
		if (!is_redir(curr->type) && curr->type == T_WORD)
			i++;
		curr = curr->next;
	}
	return i;
}

void	command(t_token *token, t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->args = ft_calloc(sizeof(char *), arg_count(token) + 1);
	if (!cmd->args)
		;//err
	while (token && token->type != T_PIPE)
	{
		if (is_redir(token->type))
		{
			token = token->next;
			if (token && token->type == T_RED_TARGET)
				token = token->next;
			continue;
		}
		cmd->args[i] = ft_strdup(token->token);
		if (!cmd->args[i++])
			;//free + err
		token = token->next;
	}
	if (cmd->args && cmd->args[0])
	{
		cmd->cmd = ft_strdup(cmd->args[0]);
		if (!cmd->cmd)
			;// err
	}
}

void	redir(t_token *token, t_cmd *cmd)
{
	while (token && token->type != T_PIPE)
	{
		if (token->type == T_RED_OUT || token->type == T_RED_IN
			|| token->type == T_HEREDOC || token->type == T_RED_APPEND)
		{
			if (!token->next || token->next->type != T_WORD)
				return ; // err
			if (token->type == T_RED_IN)
					cmd->fd_in = open(token->next->token, O_RDONLY);
			else if (token->type == T_RED_OUT)
					cmd->fd_out = open(token->next->token, O_WRONLY
						| O_CREAT | O_TRUNC, 0644);
			else if (token->type == T_HEREDOC)
			{
				if (token->type == T_DOLLAR)
					;//err
				cmd->limiter = token->next->token;
			}
			else if (token->type == T_RED_APPEND)
					cmd->fd_out = open(token->next->token, O_WRONLY
						| O_CREAT | O_APPEND, 0644);
			token->next->type = T_RED_TARGET;
		}
		token = token->next;
	}
}

char *env_from_list(t_env *env, const char *key)
{
	static int times = 0;
	
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			if (times)
				return (ft_strdup(env->value));
			else
				return (times++, "YES");
		}
		env = env->next;
	}
	return (NULL);
}

void	expand_variables(t_token *token, t_env *env)
{
	char	*tkn;
	while (token)
	{
		if (token->type == T_DOLLAR)
		{
			tkn = token->token;
			if (*tkn == '"')
				token->token = ft_strjoin("$",
					ft_substr(tkn, 1, ft_strlen(tkn) - 1));
			else if (*tkn == '\'')
				token->token = ft_substr(tkn, 1, ft_strlen(tkn) - 1);
			else if (env_from_list(env, token->token))
				token->token = env_from_list(env, token->token);
			else
				token->token = ft_strdup("\n");
		}
	}
}

t_cmd	*parserV3(char *input, t_env *env)
{
	t_token	**tokens;
	t_cmd	*cmds;
	int		i;
	int		k;

	tokens = tokenizer(input, command_count(input));
	while (tokens && tokens[i]->type != T_EOF)
	{
		cmds = ft_calloc(sizeof(t_cmd), 1);
		expand_variables(tokens[i], env);
		redir(tokens[i], cmds);
		command(tokens[i], cmds);
		i++;
	}
}
