/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 15:12:38 by aorth             #+#    #+#             */
/*   Updated: 2025/06/09 20:32:27 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_env(t_cmd *cmd, t_env *env)
{
    t_env   *env_loop;

    if (!cmd || !env)
        return (1);

    env_loop = env;
    while (env_loop)
    {
        printf("%s\n", env_loop->both);
        env_loop = env_loop->next;
    }
    return (0);
}
