/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:55:35 by ishchyro          #+#    #+#             */
/*   Updated: 2025/05/22 18:22:53 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strchr(const	char *s, int c)
{
	char	*str;

	str = (char *)s;
	while (*str != '\0')
		if (*str == (char)c || !str++)
			return (str);
	if ((char)c == '\0')
		return (str);
	return (NULL);
}

char	*quote_line(char **input, char quote)
{
	char	*out;
	char	*start;
	char	*end;
	int		len;

	start = ft_strchr(*input, quote);
	if (!start)
		return NULL;
	start++;
	end = ft_strchr(start, quote);
	if (!end)
		return NULL; //err
	len = end - start;
	out = ft_calloc(len + 1, sizeof(char));
	if (!out)
		return NULL; //err
	while (len--)
		*out++ = *start++;
	*input = (char *)(end + 1);
	return (out);
}

int	metachr(char *input)
{
	static int	i = 0;

	while (*(input + i))
	{
		if ((input[i] == '<' && *(input + i + 1) == '<')
			|| (input[i] == '>' && *(input + i + 1) == '>')
			|| (input[i] == '<') || (input[i] == '>'))
			return (i);
		i++;
	}
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*newstring;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (newstring = ft_calloc(1, 1));
	newstring = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, sizeof(char));
	i = 0;
	j = 0;
	while (s1 && s1[j] && newstring)
		newstring[i++] = s1[j++];
	j = 0;
	while (s2[j] && newstring)
		newstring[i++] = s2[j++];
	return (newstring);
}

void	fill_cmds(t_cmd *list, char *arg)
{
	if (!list->args)
	{
		list->args = ft_calloc(sizeof(char *), 3);
		list->cmd = ft_strdup(arg);
		list->args[0] = arg;
	}
	else
		list->args[1] = ft_strjoin(list->args[1], arg);
}

t_cmd	*ft_lstlast(t_cmd *list)
{
	if (!list)
		return (NULL);
	while (list->next)
		list = list->next;
	return (list);
}

void	ft_lstadd_back(t_cmd **list, t_cmd *new)
{
	t_cmd	*head;

	if (!new || !list)
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

t_cmd	*ft_lstnew(t_env *env)
{
	t_cmd	*new_node;

	new_node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_node)
		return (NULL);
	new_node->cmd = NULL;
	new_node->args = NULL;
	new_node->env = env;
	new_node->next = NULL;
	return (new_node);
}

char	*cut_line(char **input)
{
	int	size;
	int i;
	char	*res;

	size = 0;
	i = 0;
	while (*(*input + size) && (*input)[size] != '"'
		&& (*input)[size] != '\'' && (*input)[size] != ' ')
		size++;
	res = ft_calloc(sizeof(char), size + 1);
	while (i < size)
	{
		res[i] = *(*input)++;
		i++;
	}
	return (res);
}

t_cmd	*parser(char *input, t_env *env)
{
	t_cmd	*cmds;
	char	*copy;

	if (!(input && *input))
		return (NULL);
	copy = input;
	cmds = ft_lstnew(env);
	while (copy && *copy)
	{
		while (*copy == ' ' || *copy == '	')
			copy++;
		if (*copy == '"' || *copy == '\'')
			fill_cmds(ft_lstlast(cmds), quote_line(&copy, *copy));
		else
			fill_cmds(ft_lstlast(cmds), cut_line(&copy)); //finish soon
		if (copy && *copy == '|')
		{
			ft_lstadd_back(&cmds, ft_lstnew(env));
			copy++;
		}
		//printf("%c\n", *copy);
	}
	int l = 0;
	t_cmd *cmd2 = cmds;
	while (cmd2) //visualizer
	{
		printf("(cmd): %s\n", cmd2->cmd);
		while (cmd2->args[l])
		{
			printf("(arg[%d]): %s\n", l, cmd2->args[l]);
			l++;
		}
		l = 0;
		cmd2 = cmd2->next;
	}
	return (cmds);
}
