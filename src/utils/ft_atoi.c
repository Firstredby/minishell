/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:38:33 by aorth             #+#    #+#             */
/*   Updated: 2025/06/27 16:41:20 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

long long int	ft_atoi(const char *nptr)
{
	long long	result;
	int			sign;
	size_t		i;

	i = 0;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	sign = 1;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i++] == '-')
			sign *= -1;
	}
	result = 0;
	while (nptr[i] && ft_isdigit(nptr[i]))
		result = result * 10 + nptr[i++] - '0';
	if (nptr[i] && !ft_isdigit(nptr[i]))
		return (g_exit_status = 2);
	return (g_exit_status = result * sign);
}
