/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:41:24 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/09 20:01:33 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	TRASH_COLLECTOR_GOES_BRRRR(t_token **list)
{
	int i = 0;
	t_token *current;
	t_token *next;

	if (!list)
		return;

	while (list[i])
	{
		current = list[i];
		while (current)
		{
			next = current->next;
			free(current->token);
			free(current);
			current = next;
		}
		i++;
	}
	free(list);
}

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
	return (c == '\0' || c == ' ' || c == '\t'
		|| c == '<' || c == '>' || c == '|'
		|| c == '$' || c == ',');
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

void	dollar_token(char **input, t_token **list)
{
	int		i;
	char	*dollar_var;

	i = 0;
	if (**input != '$')
		return ;
	(*input)++;
	while ((ft_isalpha(**input) || ft_isdigit(**input )
		|| **input == '_') && **input != '"' && **input != '\'')
	{
		(*input)++;
		i++;
	}
	if (!i)
		return ((*input)++, addtoken(list, newtoken(ft_strdup("$"), T_WORD)));
	dollar_var = ft_substr(*input - i, 0, i);
	if (!dollar_var)
		return (perror("malloc"));//err
	addtoken(list, newtoken(dollar_var, T_DOLLAR));
}

int	token(char **input, t_token **list)
{
	char	*token;
	int		i;

	i = 0;
	if (!*input || !**input || **input == '\'' || **input == '"'
		|| **input == '$' || **input == ' ')
		return (0);
	if (**input == '<' || **input == '>')
		return (token = redirs(input, **input), 
			addtoken(list, newtoken(token, token_type(token))), 1);
	else if (**input == '|')
	{
		token = ft_strdup("|");
		if (!token)
			perror("malloc");//err
		(*input)++;
		return (addtoken(list, newtoken(token, token_type(token))), 1);
	}
	while (*(*input + i) && !ft_ismetachr(*(*input + i))
		&& *(*input + i) != '"' && *(*input + i) != '\'')
		i++;
	token = ft_substr(*input, 0, i);
	if (!token)
		perror("malloc");//err
	*input += i;
	addtoken(list, newtoken(token, token_type(token)));
	return (1);
}

int	quote_token(char **input, t_token **list)
{
	char	*token;
	int		i;
	char	quote;

	i = 1;
	if (!**(input) || (**input != '\'' && **input != '"'))
		return (0);
	quote = **input;
	while (*(*input + i) && *(*input + i) != quote)
		i++;
	if (!*(*input + i) || *(*input + i) != quote)
		return (perror("minishell: no closing quote\n"), -1);
	if (*(*input + i))
		token = ft_substr(*input, 1, i - 1);
	else
		token = ft_strdup("\"");
	*input += i + 1;
	if (i == 1)
		addtoken(list, newtoken(token, T_WORD));
	else if (quote == '\'')
		addtoken(list, newtoken(token, T_SQUOTE));
	else
		addtoken(list, newtoken(token, T_DQUOTE));
	return (1);
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
		while (*str && (*str == ' ' || *str == '\t'))
			str++;
		if (!*str)
			break ;
		dollar_token(&str, &list[index]);
		if (quote_token(&str, &list[index]))
			if (*str == '\'' || *str == '"')
				return (TRASH_COLLECTOR_GOES_BRRRR(list), NULL);
		token(&str, &list[index]);
		if (*str && *(str - 1) == '|')
			index++;
	}
	addtoken(&list[++index], newtoken(NULL, T_EOF));
	return (list);
}
