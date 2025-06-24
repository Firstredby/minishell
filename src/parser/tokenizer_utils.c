/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:41:24 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/24 12:51:52 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	addtoken(t_token **list, t_token *node)
{
	t_token	*head;

	if (!node)
		return ;
	if (!*list)
		*list = node;
	else
	{
		head = *list;
		while (head->next)
			head = head->next;
		head->next = node;
	}
}

t_token	*newtoken(char *token, t_token_type type)
{
	t_token	*new_token;
	
	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
	{
		if (token)
			free(token);
		return (perror("malloc"), NULL);
	}
	new_token->token = token;
	new_token->type = type;
	new_token->next = NULL;	
	return (new_token);
}

t_token_type	token_type(char *type)
{
	if (type == NULL)
		return (T_EOF);
	if (!ft_strcmp(type, "|"))
		return (T_PIPE);
	if (!ft_strcmp(type, "<"))
		return (T_RED_IN);
	if (!ft_strcmp(type, ">"))
		return (T_RED_OUT);
	if (!ft_strcmp(type, "<<"))
		return (T_HEREDOC);
	if (!ft_strcmp(type, ">>"))
		return (T_RED_APPEND);
	else
		return (T_WORD);
}

int	ft_ismetachr(int c)
{
	return (c == '\0' || c == '\t' || c == '$'
		|| c == '"' || c == '\'' || c == ' ');
}

void	set_space(char **input, t_token **list)
{
	char	*space;

	if (*(*input) == ' ' || *(*input) == '\t')
	{
		space = ft_strdup(" ");
		if (!space)
		{
			ft_putstr_fd("malloc error", 2);
			g_exit_status = 12;
			return;
		}
		*input += 1;
		addtoken(list, newtoken(space, T_SPACE));
	}
}
