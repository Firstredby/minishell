/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:11:36 by aorth             #+#    #+#             */
/*   Updated: 2025/05/22 15:54:58 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <complex.h>
#include <stddef.h>
#include <string.h>

char    *env_strdup(char *env, bool flag)
{
    char	*str;
	size_t	len;
    int i;
    int j;

	if (!env)
		return (NULL);
    i = 0;
    while (env[i] != '=')
        i++;
    if (flag)
    {
        len = ++i;
        i = 0;
    }
    else
	    len = ft_strlen(env) - (++i);
	str = ft_calloc(len, sizeof(char));
	if (!str)
		return (NULL);
    j = 0;
	while (env[i] && env[i] != '=')
		str[j++] = env[i++];
	return (str);
}

t_env *env_create(char *env)
{  
    t_env   *env_new;

    env_new = ft_calloc(sizeof(t_env), 1);
    if (env_new == NULL)
        return (NULL);
    env_new->both = ft_strdup(env);
    env_new->key = env_strdup(env, true);
    env_new->value = env_strdup(env, false);
    env_new->next = NULL;
    return (env_new);
}

int env_add(t_env **env_head, char *env)
{
    t_env   *env_new;
    t_env   *loop;

    env_new = env_create(env);
    if(env_new == NULL)
        return (0);
    if (*env_head == NULL)   
    {
        *env_head = env_new;
        return(1);
    }
    else
    {
    loop = *env_head;
    while (loop->next)
        loop = loop->next;
    loop->next = env_new;
    }
    return(1);
}

int    env_handle(char **env, t_env **env_head)
{
    int i;

    i = 0;
    while (env[i])
    {
        env_add(env_head, env[i]);
        i++;
    }
    return(1);
}
