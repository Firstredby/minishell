/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:57:02 by aorth             #+#    #+#             */
/*   Updated: 2025/06/27 16:26:01 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*pt;

	pt = (unsigned char *)s;
	while (n--)
	{
		*pt = (unsigned char)c;
		pt++;
	}
	return (s);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	ts;
	void	*mem;

	if (size > 0 && (nmemb > SIZE_MAX / size))
		return (NULL);
	ts = nmemb * size;
	mem = malloc(ts);
	if (!mem)
		return (NULL);
	ft_memset(mem, 0, ts);
	return (mem);
}
