/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 19:37:02 by aorth             #+#    #+#             */
/*   Updated: 2025/06/22 12:13:10 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strdup(const char *s)
{
	char	*str;
	int		i;
	size_t	len;

	if (!s)
		return (NULL);
	i = 0;
	len = ft_strlen((char*)s) + 1;
	if (len == 1)
		return NULL;
	str = ft_calloc(len, sizeof(char));
	if (!str)
		return (NULL);
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	return (str);
}
