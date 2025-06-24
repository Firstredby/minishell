/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 13:28:15 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/24 20:32:42 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_strjoin_free(char **s1, char *s2)
{
	char	*newstring;
	int		i;
	int		j;

	if (!s1 || !*s1)
	{
		if (!s2 || !*s2)
			*s1 = ft_calloc(1, 1);
		else
			*s1 = ft_strdup(s2);
		return ;
	}
	if (!s2)
		return ;
	newstring = ft_calloc(ft_strlen(*s1) + ft_strlen(s2) + 1, sizeof(char));
	if (!newstring)
		return (perror("malloc"));
	i = 0;
	j = 0;
	while ((*s1)[j])
		newstring[i++] = (*s1)[j++];
	j = 0;
	while (s2[j])
		newstring[i++] = s2[j++];
	free(*s1);
	*s1 = newstring;
}
