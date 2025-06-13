/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserV3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:09:41 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/13 21:26:16 by ishchyro         ###   ########.fr       */
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

int	cmd_size(t_token *token)
{
	int	size;

	size = 0;
	while (token && token->type != T_PIPE)
	{
		if (token->type == T_WORD)
			size++;
		token = token->next;
	}
	return (size);
}

int	lim_size(t_token *token)
{
	int	size;

	size = 0;
	while (token && token->type != T_PIPE)
	{
		if (token->type == T_HEREDOC)
			size++;
		token = token->next;
	}
	return (size);
}

void	command(t_token *token, t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->args = ft_calloc(sizeof(char *), cmd_size(token) + 1);
	if (!cmd->args)
		return (perror("malloc"));//err
	while (token && token->type != T_PIPE && token->type != T_EOF)
	{
		if (*token->token && token->type == T_WORD)
		{
			if (!ft_strcmp(token->token, " "))
				i++;
			else
				ft_strjoin_free(&cmd->args[i], token->token);
		}
		token = token->next;
	}
	if (cmd->args && cmd->args[0])
	{
		if (cmd->args[0][ft_strlen(cmd->args[0]) - 1] == ' ')
			cmd->cmd = ft_substr(cmd->args[0], 0, ft_strlen(cmd->args[0]) - 1);
		else
			cmd->cmd = ft_strdup(cmd->args[0]);
		if (!cmd->cmd)
			return (perror("malloc"));// err
	}
}

void	collect_limiter(t_token *token, t_cmd *cmd, int index)
{
	if (token->next->token[ft_strlen(token->next->token) - 1] == ' ')
		cmd->limiter[index++] = ft_substr(token->next->token, 0,
			ft_strlen(token->next->token) - 1);
	else
		cmd->limiter[index++] = ft_strdup(token->next->token);
	if (cmd->fd_in)
		close(cmd->fd_in);
	cmd->fd_in = 0;
}

void    redir(t_token *token, t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->limiter = ft_calloc(lim_size(token) + 1, sizeof(char *));
    while (token && token->type != T_PIPE)
	{
        if (token->type >= 2 && token->type <= 5)
		{
			if (!token->next || (token->next->type != 0 && token->next->type != 7))
                return (ft_putstr_fd("minishell: syntax error near unexpected token", 2)); // err
			if (token->type == T_HEREDOC)
				collect_limiter(token, cmd, i++);
            else
				open_fd(token, cmd, token->type);
			token->next->type = T_RED_TARGET;
		}
        token = token->next;
	}
}

void	expand_variables(t_token *token, t_env *env)
{
	while (token && token->type != T_PIPE)
	{
		if (token->type == T_DOLLAR)
		{
			token->token = env_from_list(env, token->token);
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
			while (!ft_ismetachr(token->token[i + k])
				&& token->token[i + k] != '|' && token->token[i + k] != ' ')
				k++;
			if (k == 0)
				token->token = replace_string(token->token, NULL, i, k);
			else
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
			tmp = ft_substr(token->token, 0, ft_strlen(token->token));
			if (!tmp)
				return (1); //err
			free(token->token);
			token->token = tmp;
			token->type = T_WORD;
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
