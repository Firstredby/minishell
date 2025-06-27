/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_strcmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:17:53 by aorth             #+#    #+#             */
/*   Updated: 2025/06/27 16:26:45 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env_strcmp(const char *s1, const char *s2)
{
	s1++;
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
