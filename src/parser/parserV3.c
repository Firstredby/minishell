/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserV3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:09:41 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/09 19:44:43 by aorth            ###   ########.fr       */
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

int	arg_countV3(t_token *token)
{
	t_token	*curr;
	int	i;

	curr = token;
	i = 0;
	while (curr && curr->type != T_PIPE)
	{
		if (curr->type == T_WORD)
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
	while (token && token->type != T_PIPE && token->type != T_EOF)
	{
		if (*token->token && token->type == T_WORD)
		{
			cmd->args[i] = ft_strdup(token->token);
			if (!cmd->args[i++])
				perror("malloc");//free + err	
		}
		token = token->next;
	}
	if (cmd->args && cmd->args[0])
	{
		cmd->cmd = ft_strdup(cmd->args[0]);
		if (!cmd->cmd)
			perror("malloc");// err
	}
}

int	limiter_size(t_token *token)
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

void	open_fd(t_token *token, t_cmd *cmd, int redir)
{
    int flags;
	int fd;

    if (redir == T_RED_IN)
	{
		flags = O_RDONLY;
		fd = cmd->fd_in;
	}
    else
		fd = cmd->fd_out;
	if (redir == T_RED_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
    if (fd)
		close(fd);
    cmd->fd_out = open(token->next->token, flags, 0644);
	if (cmd->fd_out == -1)
        perror("fd");//err
}

void    redir(t_token *token, t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->limiter = ft_calloc(limiter_size(token) + 1, sizeof(char *));
    while (token && token->type != T_PIPE)
	{
        if (token->type == T_RED_IN || token->type == T_HEREDOC
			|| token->type == T_RED_APPEND || token->type == T_RED_OUT)
		{
			if (!token->next || (token->next->type != T_WORD
				&& token->next->type != T_DOLLAR))
			{
                return ; // err
			}
			if (token->type == T_HEREDOC)
            {
				cmd->limiter[i++] = ft_strdup(token->next->token);
                if (cmd->fd_in)
				{
					close(cmd->fd_in);
				}
                cmd->fd_in = 0;
			}
            else if(token->type != T_HEREDOC)
			{
				open_fd(token, cmd, token->type);
			}
			token->next->type = T_RED_TARGET;
		}
        token = token->next;
	}
}

char *env_from_list(t_env *env, char *key)
{
	t_env *head;

	head = env;
	while (head)
	{
		if (!ft_strcmp(head->key, key))
		{
			free(key);
			key = ft_strdup(head->value);
			if (!key)
				;//err
			return (key);
		}
		head = head->next;
	}
	free(key);
	key = ft_calloc(1, 1);
	if (!key)
		;//err
	return (key);
}

void	expand_variables(t_token *token, t_env *env)
{
	while (token && token->type != T_PIPE)
	{
		if (token->type == T_DOLLAR)
		{
			if (*token->token == '\'' || *token->token == '"')
				token->token = ft_substr(token->token, 1, ft_strlen(token->token) - 2);
			token->token = env_from_list(env, token->token);
			if (!token->token)
				perror("malloc");
			token->type = T_WORD;
		}
		token = token->next;
	}
}

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
		;//err
	while (str_pos < i - 1)
	{
		newstr[str_pos] = str[str_pos];
		str_pos++;
	}
	while (var[var_pos])
		newstr[str_pos++] = var[var_pos++];
	while (str[i + k])
		newstr[str_pos] = str[i + k++];
	(free(str), free(var));
	str = newstr;
	return (newstr);
}

void	expand_quotes(t_token *token, t_env *env)
{
	int	i;
	int	k;
	
	while (token && token->type != T_EOF)
	{
		i = 0;
		k = 0;
		if (token->type == T_DQUOTE)
		{
			while (token->token[i] && token->type == T_DQUOTE)
			{
				if (token->token[i++] == '$')
				{
					while (!ft_ismetachr(token->token[i + k]))
						k++;
					token->token = replace_string(token->token, 
					env_from_list(env, ft_substr(token->token, i, k)), i, k);
					break ;
				}
			}
			if (ft_strlen(token->token) != (size_t)i)
				continue ;
			else
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
	if (!tokens)
		return (NULL);
	i = 0;
	cmds = ft_calloc(sizeof(t_cmd), 1);
	head = cmds;
	while (tokens && tokens[i]->type != T_EOF)
	{
		expand_quotes(tokens[i], env);
		expand_variables(tokens[i], env);
		redir(tokens[i], cmds);
		//check for spaces to put them in
		command(tokens[i], cmds);
		i++;
		if (tokens[i] && tokens[i]->type != T_EOF)
		{
			cmds->next = ft_calloc(sizeof(t_cmd), 1);
			cmds = cmds->next;
		}
		//cmds->next = NULL;
	}
	TRASH_COLLECTOR_GOES_BRRRR(tokens);
	return (head);
}
