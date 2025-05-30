/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 21:29:15 by aorth             #+#    #+#             */
/*   Updated: 2025/05/15 16:09:23 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*dpt;
	unsigned char	*spt;

	if (!dest && !src)
		return ;
	dpt = (unsigned char *)dest;
	spt = (unsigned char *)src;
	while (n--)
	{
		*dpt++ = *spt++;
	}
	dpt = NULL;
	spt = NULL;
	return ;
}
