/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   looking_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:04:33 by aorth             #+#    #+#             */
/*   Updated: 2025/06/29 14:01:43 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	looking_path(t_cmd *cmd, char **env)
{
	int	i;

	(void)cmd;
	i = 0;
	if(!env || !*env || !**env)
		return (0);
	while (env[i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
		{
			if (ft_strstr(env[i], "/bin:") || ft_strstr(env[i], ":/bin:")
				|| ft_strstr(env[i], ":/bin") || ft_strstr(env[i], "/bin"))
				return (1);
			return (0);
		}
		else if (!ft_strncmp(env[i], "PWD=", 4))
		{
			if (ft_strstr(env[i], "/bin:") || ft_strstr(env[i], ":/bin:")
				|| ft_strstr(env[i], ":/bin") || ft_strstr(env[i], "/bin"))
				return (1);
		}
		i++;
	}
	return (0);
}
