/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:02:20 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/27 16:25:22 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strchr(const	char *s, int c)
{
	char	*str;
	int		i;

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
