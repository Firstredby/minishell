/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_strcmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:17:53 by aorth             #+#    #+#             */
/*   Updated: 2025/05/22 14:39:33 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int env_strcmp(const char *s1, const char *s2)
{
    s1++;
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}