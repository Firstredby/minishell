/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 15:12:38 by aorth             #+#    #+#             */
/*   Updated: 2025/06/27 15:10:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(t_cmd *cmd, t_env *env)
{
	t_env	*env_loop;

	if (!cmd || !env)
		return (1);
	env_loop = env;
	while (env_loop)
	{
		if (env_loop->both)
			printf("%s\n", env_loop->both);
		env_loop = env_loop->next;
	}
	return (g_exit_status = 0);
}
