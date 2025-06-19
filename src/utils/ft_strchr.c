/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:02:20 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/19 15:43:45 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strchr(const	char *s, int c)
{
	char	*str;
	int i;

	i = 0;
	str = (char *)s;
	while (*str != '\0')
	{
		if (*str == (char)c)
			i++;
		str++;
	}
	return (i);
}
