/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_filename.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:38:17 by aorth             #+#    #+#             */
/*   Updated: 2025/06/27 16:24:10 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// "/tmp/heredoc"+"[i]"+".tmp"

static void	ft_strroids(char *tmp, char *path, char *id, char *type)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (path[i])
	{
		tmp[i] = path[i];
		i++;
	}
	while (id[j])
		tmp[i++] = id[j++];
	j = 0;
	while (type[j])
		tmp[i++] = type[j++];
}

char	*create_filename(char *path, char *id, char *type)
{
	size_t	size;
	char	*temp;

	size = ft_strlen(path) + ft_strlen(id) + ft_strlen(type);
	temp = ft_calloc(size +1, sizeof(char));
	if (!temp)
		return (NULL);
	ft_strroids(temp, path, id, type);
	return (temp);
}
