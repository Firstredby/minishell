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
	return (result + 1);
}

bool	is_redir(t_token *token)
{
	if (token->type == T_RED_OUT || token->type == T_RED_IN
		|| token->type == T_HEREDOC || token->type == T_RED_APPEND)
		return (true);
	return (false);
}

int	arg_countV3(t_token *token)
{
	t_token	*curr;
	int	i;

	curr = token;
	i = 0;
	while (curr && curr->type != T_PIPE)
	{
		if (!is_redir(curr) && curr->type == T_WORD)
			i++;
		curr = curr->next;
	}
	return i;
}

void	command(t_token *token, t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->args = ft_calloc(sizeof(char *), arg_countV3(token) + 1);
	if (!cmd->args)
		perror("malloc");//err
	while (token && token->type != T_PIPE)
	{
		if (is_redir(token))
		{
			token = token->next->next;
			continue;
		}
		cmd->args[i] = ft_strdup(token->token);
		if (!cmd->args[i++])
			perror("malloc");//free + err
		token = token->next;
	}
	if (cmd->args && cmd->args[0])
	{
		cmd->cmd = ft_strdup(cmd->args[0]);
		if (!cmd->cmd)
			perror("malloc");// err
	}
}

int	open_fd(t_token *token, int redir, int *curr_fd)
{
    int flags;

    if (redir == T_RED_IN)
		flags = O_RDONLY;
    else if (redir == T_RED_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
    else
		flags = O_WRONLY | O_CREAT | O_APPEND;
    if (*curr_fd)
		close(*curr_fd);
    *curr_fd = open(token->next->token, flags, 0644);
	if (*curr_fd == -1)
        perror("fd");//err
	return (*curr_fd);
}

void    redircopy(t_token *token, t_cmd *cmd)
{
    while (token && token->type != T_PIPE)
	{
        if (token->type >= 2 && token->type <= 5)
		{
            if (!token->next || token->next->type != T_WORD
				|| (token->type == T_HEREDOC && token->next->type == T_DOLLAR))
                return ; // err            
			if (token->type == T_RED_IN)
                cmd->fd_in = open_fd(token, T_RED_IN, &cmd->fd_in);
			else if (token->type == T_HEREDOC)
            {
				cmd->limiter = token->next->token;
                if (cmd->fd_in)
					close(cmd->fd_in);
                cmd->fd_in = 0;
			}
            else if (token->type == T_RED_OUT)
				cmd->fd_out = open_fd(token, T_RED_OUT, &cmd->fd_out);
            else if (token->type == T_RED_APPEND)
				cmd->fd_out = open_fd(token, T_RED_APPEND, &cmd->fd_out);
            token->next->type = T_RED_TARGET;
		}
        token = token->next;
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
					perror("Invalid variable");//err
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
	t_env *head;

	head = env;
	while (head)
	{
		if (!ft_strcmp(head->key, key))
			return (ft_strdup(head->value));
		head = head->next;
	}
	return (NULL);
}

bool is_env_var(t_env *env, const char *key)
{
	t_env *head;

	head = env;
	while (head)
	{
		if (!ft_strcmp(head->key, key))
			return (true);
		head = head->next;
	}
	return (false);
}

void	expand_variables(t_token *token, t_env *env)
{
	char	*tkn;
	while (token && token->type != T_PIPE)
	{
		if (token->type == T_DOLLAR)
		{
			tkn = token->token;
			if (*tkn == '"')
				token->token = ft_strjoin("$",
					ft_substr(tkn, 1, ft_strlen(tkn) - 2));
			else if (*tkn == '\'')
				token->token = ft_substr(tkn, 1, ft_strlen(tkn) - 2);
			else if (is_env_var(env, token->token))
				token->token = env_from_list(env, token->token);
			else
				token->token = ft_strdup("\n");
			token->type = T_WORD;
		}
		token = token->next;
	}
}

t_cmd	*parserV3(char *input, t_env *env)
{
	t_token	**tokens;
	t_cmd	*cmds;
	t_cmd	*head;
	int		i;

	tokens = tokenizerV3(input, command_count(input));
	i = 0;
	cmds = ft_calloc(sizeof(t_cmd), 1);
	head = cmds;
	while (tokens && tokens[i]->type != T_EOF)
	{
		expand_variables(tokens[i], env);
		redircopy(tokens[i], cmds);
		command(tokens[i], cmds);
		i++;
		if (tokens[i]->type != T_EOF)
		{
			cmds->next = ft_calloc(sizeof(t_cmd), 1);
			cmds = cmds->next;
		}
	}
	return (head);
}
