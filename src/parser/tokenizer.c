/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:41:24 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/29 17:47:03 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdio.h>

char	*redirs(char **input, char sign)
{
	char	*res;

	if (**input == sign)
	{
		if (*(*input + 1) == **input)
		{
			res = ft_substr(*input, 0, 2);
			if (!res)
			{
				ft_putstr_fd("malloc error", 2);
				g_exit_status = 12;
			}
			(*input) += 2;
			return (res);
		}
		res = ft_substr(*input, 0, 1);
		if (!res)
		{
			ft_putstr_fd("malloc error", 2);
			g_exit_status = 12;
		}
		(*input)++;
		return (res);
	}
	return (NULL);
}

int	dollar_token(char **input, t_token **list)
{
	int		i;
	char	*dollar_var;
	char	*dollar_str;

	i = 1;
	dollar_str = NULL;
	if (**input != '$')
		return (0);
	while (ft_isalnum(*(*input + i)) || *(*input + i) == '_'
		|| *(*input + i) == '?')
		i++;
	if (i == 1 && (*(*input + i) == '"' || *(*input + i) == '\''))
		return ((*input)++, addtoken(list,
				newtoken(ft_calloc(1, 1), T_WORD)), 0);
	else if (i == 1)
	{
		dollar_str = ft_strdup("$");
		if (!dollar_str)
			return (ft_putstr_fd("malloc error", 2), g_exit_status = 12, 1);
		return ((*input)++, addtoken(list,
				newtoken(dollar_str, T_WORD)), 0);
	}
	dollar_var = ft_substr(*input, 1, i - 1);
	if (!dollar_var)
		return (ft_putstr_fd("malloc error", 2), g_exit_status = 12, 1);
	addtoken(list, newtoken(dollar_var, T_DOLLAR));
	*input += i;
	set_space(input, list);
	return (0);
}

int	token(char **input, t_token **list)
{
	char	*token;
	int		i;

	i = 0;
	if (!*input || ft_ismetachr(**input))
		return (0);
	if (**input == '<' || **input == '>')
		return (token = redirs(input, **input),
			addtoken(list, newtoken(token, token_type(token))), 0);
	else if (**input == '|')
	{
		token = ft_strdup("|");
		if (!token)
			return (ft_putstr_fd("malloc error", 2), g_exit_status = 12, 1);
		(*input)++;
		return (addtoken(list, newtoken(token, token_type(token))), 0);
	}
	while (!ft_ismetachr(*(*input + i)) && *(*input + i) != '>' && *(*input + i) != '<' && *(*input + i) != '|')
		i++;
	token = ft_substr(*input, 0, i);
	if (!token)
		return (ft_putstr_fd("malloc error", 2), g_exit_status = 12, 1);
	*input += i;
	addtoken(list, newtoken(token, token_type(token)));
	return (set_space(input, list), 0);
}

int	quote_token(char **input, t_token **list)
{
	char	*token;
	int		i;

	i = 1;
	if (!**(input) || (**input != '\'' && **input != '"'))
		return (0);
	while (*(*input + i) && *(*input + i) != **input)
		i++;
	if (!*(*input + i) || *(*input + i) != **input)
		return (miss_quote(), 1);
	if (*(*input + i))
		token = ft_substr(*input, 1, i - 1);
	else
		token = ft_calloc(1, 1);
	if (!token)
		return (ft_putstr_fd("malloc error", 2), g_exit_status = 12, 1);
	if (i == 1)
		addtoken(list, newtoken(token, T_WORD));
	else if (**input == '\'')
		addtoken(list, newtoken(token, T_SQUOTE));
	else
		addtoken(list, newtoken(token, T_DQUOTE));
	*input += i + 1;
	set_space(input, list);
	return (0);
}

t_token	**tokenizer(char *input, size_t size)
{
	t_token	**list;
	size_t	index;
	char	*str;

	list = (t_token **)ft_calloc(sizeof(t_token *), (size + 2));
	if (!list)
		return (g_exit_status = 12, NULL);
	index = 0;
	str = input;
	while (str && *str)
	{
		while (*str && (*str == ' ' || *str == '\t'))
			str++;
		if (token(&str, &list[index]))
			return (NULL);
		if (*str && str > input && *(str - 1) == '|')
			index++;
		if (dollar_token(&str, &list[index]))
			return (NULL);
		if (quote_token(&str, &list[index]))
			return (NULL);
	}
	addtoken(&list[++index], newtoken(NULL, T_EOF));
	return (list);
}
