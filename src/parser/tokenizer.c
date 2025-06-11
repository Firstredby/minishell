/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:41:24 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/11 22:34:48 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*redirs(char **input, char sign)
{
	char	*res;

	if (**input == sign)
	{
		if (*(*input + 1) == **input)
		{
			res = ft_substr(*input, 0, 2);
			if (!res)
				perror("malloc");//err
			(*input) += 2;
			return (res);
		}
		res = ft_substr(*input, 0, 1);
		if (!res)
			perror("malloc");//err
		(*input)++;
		return (res);
	}
	return (NULL);
}

int	dollar_token(char **input, t_token **list)
{
	int		i;
	char	*dollar_var;

	i = 1;
	if (**input != '$')
		return (0);
	while ((ft_isalnum(*(*input + i)) || *(*input + i) == '_')
		|| *(*input + i) == '?')
		i++;
	if (i == 1 && (*(*input + i) == '"' || *(*input + i) == '\''))
		return ((*input)++, addtoken(list,
			newtoken(ft_calloc(1, 1), T_WORD)), 0);
	else if (i == 1)
		return ((*input)++, addtoken(list,
			newtoken(ft_strdup("$"), T_WORD)), 0);
	dollar_var = ft_substr(*input, 1, i);
	if (!dollar_var)
		return (perror("malloc"), TRASH_COLLECTOR_GOES_BRRRR(list), 1);//err
	addtoken(list, newtoken(dollar_var, T_DOLLAR));
	return (*input += i, 0);
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
			return (perror("malloc"), TRASH_COLLECTOR_GOES_BRRRR(list), 1);//err
		(*input)++;
		return (addtoken(list, newtoken(token, token_type(token))), 0);
	}
	while (!ft_ismetachr(*(*input + i)))
		if (*(*input + i++) == ' ')
			break ;
	token = ft_substr(*input, 0, i);
	if (!token)
		return (perror("malloc"), TRASH_COLLECTOR_GOES_BRRRR(list), 1);//err
	*input += i;
	return (addtoken(list, newtoken(token, token_type(token))), 0);
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
		return (perror("minishell: no closing quote\n"),
		TRASH_COLLECTOR_GOES_BRRRR(list), 1);
	if (*(*input + i))
		token = ft_substr(*input, 1, i - 1);
	else
		token = ft_strdup("\"");
	if (i == 1)
		addtoken(list, newtoken(token, T_WORD));
	else if (**input == '\'')
		addtoken(list, newtoken(token, T_SQUOTE));
	else
		addtoken(list, newtoken(token, T_DQUOTE));
	*input += i + 1;
	return (0);
}

t_token	**tokenizerV3(char *input, size_t size)
{
	t_token	**list;
	size_t	index;
	char	*str;

	list = (t_token **)ft_calloc(sizeof(t_token *), (size + 2));
	index = 0;
	str = input;
	while (str && *str)
	{
		while (*str && *str == ' ')
			str++;
		//printf("skip to %s\n", str);
		if (token(&str, &list[index]))
			return (NULL);
		//printf("after token %s\n", str);
		if (*str && *(str - 1) == '|')
			index++;
		if (dollar_token(&str, &list[index]))
			return (NULL);
		if (quote_token(&str, &list[index]))
			return (NULL);
	}
	addtoken(&list[++index], newtoken(NULL, T_EOF));
	return (list);
}
