/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:25:06 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/27 16:06:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	double_pipe(t_token **list)
{
	int		i;
	t_token	*head;

	i = 0;
	while (list[i]->type != T_EOF)
	{
		head = list[i];
		if (head->token && !ft_strcmp(head->token, "|"))
		{
			syn_err(head);
			return (1);
		}
		while (head->next)
			head = head->next;
		if (head->token && !ft_strcmp(head->token, "|")
			&& list[i + 1]->type == T_EOF)
		{
			syn_err(head);
			return (1);
		}
		i++;
	}
	return (0);
}

bool	is_redir(t_token_type type)
{
	return (type == T_RED_IN || type == T_RED_APPEND
		|| type == T_HEREDOC || type == T_RED_OUT);
}

int	redir_check(t_token *curr)
{
	if (is_redir(curr->type) && (!curr->next
			|| (curr->next->type != 0 && curr->next->type != 7
				&& curr->next->type != 9 && curr->next->type != 10)))
		return (1);
	return (0);
}

void	open_heredoc(t_token **src, t_token *end)
{
	char	*line;
	t_token	*begin;
	int		i;

	i = 0;
	begin = src[i];
	while (src[i + 1]->type != T_EOF && begin != end)
	{
		begin = src[i++];
		while (begin)
		{
			while (begin->type == 5)
			{
				line = readline("> ");
				if (!ft_strcmp(line, begin->next->token) || !line)
					break ;
				free(line);
			}
			begin = begin->next;
		}
	}
}

int	parser_validator(t_token **token)
{
	int		i;
	t_token	*curr;

	i = 0;
	if (double_pipe(token))
		return (1);
	while (token[i] && token[i]->type != T_EOF)
	{
		curr = token[i];
		while (curr)
		{
			if (redir_check(curr))
			{
				syn_err(curr->next);
				open_heredoc(token, curr);
				return (1);
			}
			curr = curr->next;
		}
		i++;
	}
	return (0);
}
