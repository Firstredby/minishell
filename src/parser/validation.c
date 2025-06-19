/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:25:06 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/19 20:48:03 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool double_pipe(t_token **list)
{
	int	i;
	t_token *head;

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

int	parser_validator(t_token **token)
{
	int	i;
	t_token *curr;

	i = 0;
	if (double_pipe(token))
		return (1);
	while (token[i] != T_EOF)
	{
		curr = token[i];
		while (curr)
		{
			if (curr->token == "=")
				return (ft_putstr_fd("bad assigment\n", 2), 1);
			if (is_redir(curr->type) && !curr->next
				|| (curr->next->type != 0 && curr->next->type != 7))
                return (syn_err(curr->next), 1);
		}
		i++;
	}	
}
