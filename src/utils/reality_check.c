/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reality_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 00:21:31 by aorth             #+#    #+#             */
/*   Updated: 2025/06/30 01:08:21 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	reality_check(char *input)
{
	if (!input)
		return (0);
	if (input[0] == '\0')
	{
		free(input);
		return (1);
	}
	return (2);
}
