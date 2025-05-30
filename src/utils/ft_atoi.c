/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:38:33 by aorth             #+#    #+#             */
/*   Updated: 2025/05/15 16:01:17 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


static void error_exit(const char *msg) {
    printf("Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

static int	is_valid_help(long temp)
{
	if (temp < -2147483648 || temp > 2147483647)
		return (0);
	return (1);
}

static int	atoi_helper(const char *str, int n)
{
	int	i;

	i = n;
	while (str[i] == '0')
		i++;
	if (ft_strlen(str) - i > 10)
		return (0);
	return (1);
}

long	ft_atoi(const char *nptr)
{
	long	result;
	int		sign;
	int		i;

	i = 0;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	sign = 1;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i++] == '-')
			sign *= -1;
	}
	if (!ft_isdigit(nptr[i]))
		error_exit("Agrument is not a number");
	if (!atoi_helper(nptr, i))
		error_exit("Argument is too long");
	result = 0;
	while (nptr[i] && ft_isdigit(nptr[i]))
		result = result * 10 + nptr[i++] - '0';
	if (nptr[i] && !ft_isdigit(nptr[i]))
		return (0);
	if (!is_valid_help(result))
		error_exit("Argument is too long");
	return (result * sign);
}
