/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:33:46 by aorth             #+#    #+#             */
/*   Updated: 2025/06/06 10:41:59 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_getlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static void	ft_fill(char *str, int n, int size)
{
	int	index;

	index = size - 1;
	while (index >= 0)
	{
		str[index] = n % 10 + '0';
		n /= 10;
		index--;
	}
}

char	*ft_itoa(int n)
{
	int		start;
	int		size;
	char	*str;

	size = ft_getlen(n);
	str = (char *)malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	str[size] = '\0';
	start = 0;
	if (n == -2147483648)
	{
		str[start++] = '-';
		str[start++] = '2';
		n = 147483648;
	}
	else if (n < 0)
	{
		str[start++] = '-';
		n = -n;
	}
	ft_fill(str + start, n, size - start);
	return (str);
}
