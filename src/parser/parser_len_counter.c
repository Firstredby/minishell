/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_len_counter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 19:51:20 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/29 19:51:48 by ishchyro         ###   ########.fr       */
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
		if (token->type != T_SPACE && !is_redir(token->type))
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
