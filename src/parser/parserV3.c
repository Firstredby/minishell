/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserV3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:09:41 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/10 22:42:10 by codespace        ###   ########.fr       */
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
	return (result + 1);
}

int	type_size(t_token *token, t_token_type type)
{
	int	size;

	size = 0;
	while (token && token->type != T_PIPE)
	{
		if (token->type == type)
			size++;
		token = token->next;
	}
	return (size);
}

void	command(t_token *token, t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->args = ft_calloc(sizeof(char *), type_size(token, token->type) + 1);
	if (!cmd->args)
		return (perror("malloc"));//err
	while (token && token->type != T_PIPE && token->type != T_EOF)
	{
		if (*token->token && token->type == T_WORD)
		{
			cmd->args[i] = ft_strdup(token->token);
			if (!cmd->args[i++])
				return (perror("malloc"));//free + err	
		}
		token = token->next;
	}
	if (cmd->args && cmd->args[0])
	{
		cmd->cmd = ft_strdup(cmd->args[0]);
		if (!cmd->cmd)
			return (perror("malloc"));// err
	}
}

void    redir(t_token *token, t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->limiter = ft_calloc(type_size(token, token->type) + 1, sizeof(char *));
    while (token && token->type != T_PIPE)
	{
        if (token->type == T_RED_IN || token->type == T_HEREDOC
			|| token->type == T_RED_APPEND || token->type == T_RED_OUT)
		{
			if (!token->next || (token->next->type != T_WORD
				&& token->next->type != T_DOLLAR))
                return (ft_putstr_fd("minishell: syntax error near unexpected token", 2)); // err
			if (token->type == T_HEREDOC)
            {
				cmd->limiter[i++] = ft_strdup(token->next->token);
                if (cmd->fd_in)
					close(cmd->fd_in);
                cmd->fd_in = 0;
			}
            else
				open_fd(token, cmd, token->type);
			token->next->type = T_RED_TARGET;
		}
        token = token->next;
	}
}

void	expand_variables(t_token *token, t_env *env)
{
	char	*tmp;

	while (token && token->type != T_PIPE)
	{
		if (token->type == T_DOLLAR)
		{
			tmp = ft_substr(token->token, 1, ft_strlen(token->token) - 2);
			if (!tmp)
				return ;
			if (*token->token == '\'')
			{
				free(token->token);
				token->token = tmp;
			}
			else if (*token->token == '"')
			{
				free(token->token);
				token->token = env_from_list(env, tmp);
			}
			if (!token->token)
				return (perror("malloc"));
			token->type = T_WORD;
		}
		token = token->next;
	}
}

char	*dquote_expansion(t_token *token, t_env *env)
{
	int		i;
	int		k;

	i = 0;
	k = 0;
	while (token->token[i])
		if (token->token[i++] == '$')
		{
			while (!ft_ismetachr(token->token[i + k]))
				k++;
			token->token = replace_string(token->token, 
				env_from_list(env, ft_substr(token->token, i, k)), i, k);
			if (!token->token)
				return (NULL);
			break ;
		}
	if (ft_strlen(token->token) != (size_t)i)
		return (dquote_expansion(token, env));
	else
		return (token->token);
}

int	expand_quotes(t_token *token, t_env *env)
{
	char	*tmp;

	while (token && token->type != T_EOF)
	{
		if (token->type == T_DQUOTE)
		{
			if (!dquote_expansion(token, env))
				return (1);
			else
				token->type = T_WORD;
		}
		else if (token->type == T_SQUOTE)
		{
			tmp = ft_substr(token->token, 1, ft_strlen(token->token) - 1);
			if (!tmp)
				return (1); //err
			free(token->token);
			token->token = tmp;
		}
		token = token->next;
	}
	return (0);
}

t_cmd	*parserV3(t_token **tokens, t_env *env)
{
	t_cmd	*cmds;
	t_cmd	*head;
	int		i;

	i = 0;
	cmds = ft_calloc(sizeof(t_cmd), 1);
	if (!cmds)
		return (NULL);
	head = cmds;
	while (tokens && tokens[i]->type != T_EOF)
	{
		cmds->env = env;
		expand_quotes(tokens[i], env);
		expand_variables(tokens[i], env);
		redir(tokens[i], cmds);
		//check for spaces to put them in
		command(tokens[i], cmds);
		//printf("i'm here\n");
		if (tokens[++i] && tokens[i]->type != T_EOF)
		{
			cmds->next = ft_calloc(sizeof(t_cmd), 1);
			if (!cmds)
				return (head);
			cmds = cmds->next;
		}
	}
	return (TRASH_COLLECTOR_GOES_BRRRR(tokens), head);
}
