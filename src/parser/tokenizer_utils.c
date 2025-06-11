/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:41:24 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/11 22:06:29 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	addtoken(t_token **list, t_token *new)
{
	t_token	*head;

	if (!new)
		return ;
	if (!*list)
		*list = new;
	else
	{
		head = *list;
		while (head->next)
			head = head->next;
		head->next = new;
	}
}

t_token	*newtoken(char *token, t_token_type type)
{
	t_token	*new_token;
	
	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		perror("malloc");//err
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
		|| c == '"' || c == '\'');
}