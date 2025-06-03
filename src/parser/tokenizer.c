/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:41:24 by ishchyro          #+#    #+#             */
/*   Updated: 2025/05/22 21:01:35 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	int		i;
	char	*substr;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (substr = ft_calloc(1, 1));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	substr = ft_calloc(len + 1, sizeof(char));
	if (!substr)
		perror("malloc");//err;
	i = 0;
	while (i++ < (int)len)
		substr[i - 1] = (char)s[i + start - 1];
	return (substr);
}

int	ft_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z'));
}

int	ft_ismetachr(int c)
{
	return (c == '\0' || c == ' ' || c == '\t' ||
		c == '<' || c == '>' || c == '|' || c == '$');
}

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

char	*dollar_token(char **input)
{
	int		i;
	char	*dollar_var;

	i = 0;
	if (**input != '$')
		return (NULL);
	(*input)++;
	while (ft_isalpha(**input) || ft_isdigit(**input)
		|| **input == '_' || **input == '"' || **input == '\'')
	{
		(*input)++;
		i++;
	}
	dollar_var = ft_substr(*input - i, 0, i);
	if (!dollar_var)
		perror("malloc");//err
	return (dollar_var);
}

char	*token(char **input)
{
	char	*token;
	int		i;

	i = 0;
	if (!*input || !**input)
		return NULL;
	if (**input == '<' || **input == '>')
		return (redirs(input, **input));
	if (**input == '|')
	{
		token = ft_strdup("|");
		if (!token)
			perror("malloc");//err
		(*input)++;
		return (token);
	}
	while (*(*input + i) && !ft_ismetachr(*(*input + i)))
		i++;
	token = ft_substr(*input, 0, i);
	if (!token)
		perror("malloc");//err
	return (token);
}

char	*quote_token(char **input)
{
	char	*token;
	int		i;
	char	quote;

	i = 1;
	if (**(input) && **input != '\'' && **input != '"')
		return (NULL);
	quote = **input;
	while (*(*input + i) && *(*input + i) != quote)
		i++;
	if (!*(*input + i) && *(*input + i) != quote)
		return (perror("no closing quote"), NULL);
	token = ft_substr(*input, 0, i + 1);
	*input += i + 1;
	return (token);
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

t_token	**tokenizerV3(char *input, size_t size)
{
	t_token	**list;
	size_t	index;
	char	*tkn;

	list = (t_token **)ft_calloc(sizeof(t_token *), (size + 2));
	index = 0;
	while (*input)
	{
		while (*input && (*input == ' ' || *input == '\t'))
			input++;
		tkn = dollar_token(&input);
		if (tkn)
			addtoken(&list[index], newtoken(tkn, T_DOLLAR));
		tkn = quote_token(&input);
		if (tkn)
		{
			addtoken(&list[index], newtoken(tkn, T_QUOTE));
			continue ;
		}
		addtoken(&list[index], newtoken(token(&input), token_type(tkn)));
		if (tkn && !ft_strcmp(tkn, "|"))
			index++;
	}
	addtoken(&list[++index], newtoken(NULL, T_EOF));
	return (list);
}
