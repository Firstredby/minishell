/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:11:36 by aorth             #+#    #+#             */
/*   Updated: 2025/06/27 21:44:29 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <complex.h>
#include <stddef.h>
#include <string.h>

char	*no_more_spaces(char *env, int pos)
{
	char	*res;
	int		i;
	int		size;

	i = pos;
	size = 0;
	while (env[i])
	{
		while (env[i] == ' ' && env[i + 1] == ' ')
			i++;
		size++;
		i++;
	}
	res = ft_calloc(sizeof(char), size + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (env[pos])
	{
		while (env[pos] == ' ' && env[pos + 1] == ' ')
			pos++;
		res[i++] = env[pos++];
	}
	return (res);
}

char	*env_strdup(char *env, bool flag)
{
	char	*str;
	int		i;
	int		j;

	if (!env)
		return (NULL);
	i = 0;
	while (env[i] != '=' && env[i])
		i++;
	if (flag)
		str = ft_calloc(++i + 1, sizeof(char));
	else if (env[i] == '=')
		return (no_more_spaces(env, ++i));
	else
		return (ft_substr(env, 0, i));		
	if (!str)
		return (NULL);
	j = 0;
	i = 0;
	while (env[i] && env[i] != '=')
		str[j++] = env[i++];
	return (str);
}

t_env	*env_create(char *env)
{
	t_env	*env_new;

	env_new = ft_calloc(sizeof(t_env), 1);
	if (env_new == NULL)
		return (NULL);
	env_new->key = env_strdup(env, true);
	if (!env_new->key)
		return (free(env_new), NULL);
	if (ft_strchr(env, '=') != 0)
	{
		env_new->both = ft_strdup(env);
		if (!env_new->both)
			return (free(env_new->key), free(env_new), NULL);
		env_new->value = env_strdup(env, false);
		if (!env_new->value)
			return (free(env_new->both),
				free(env_new->key),
				free(env_new), NULL);
	}
	env_new->next = NULL;
	return (env_new);
}

int	env_add(t_env **env_head, char *env)
{
	t_env	*env_new;
	t_env	*loop;

	env_new = env_create(env);
	if (env_new == NULL)
		return (0);
	if (*env_head == NULL)
		*env_head = env_new;
	else
	{
		loop = *env_head;
		while (loop->next)
			loop = loop->next;
		loop->next = env_new;
		env_new->next = NULL;
	}
	return (1);
}

int	env_handle(char **env, t_env **env_head)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!env_add(env_head, env[i]))
			return (0);
		i++;
	}
	return (1);
}
