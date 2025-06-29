/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 17:59:48 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/29 18:05:30 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free2d(char **list)
{
	int	i;

	if (!list)
		return ;
	i = 0;
	while (list[i])
	{
		free(list[i]);
		list[i] = NULL;
		i++;
	}
	free(list);
}

void	closefd(int fd)
{
	if (fd > 2)
		close(fd);
}

void	free_all(t_cmd *cmd, t_env *env, t_token **token)
{
	if (cmd)
		cmd_cleaner(cmd);
	if (env)
		env_cleaner(env);
	if (token)
		trash_collector_goes_brrrr(token);
}
