/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ll_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 20:17:06 by aorth             #+#    #+#             */
/*   Updated: 2025/05/20 23:15:41 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ll_free(t_cmd *cmd)
{
	// t_cmd	*cmd_present;
    // t_cmd	*cmd_curr;

	if (!cmd)
		return ;
	// cmd_present = cmd;
	// while (cmd_present)
    // {
    //     free(cmd_present->cmd);
    //     ft_free_split(cmd_present->args);
    //     cmd_curr = cmd_present;
    //     cmd_present = cmd_present->next;
    //     free(cmd_curr);
    //     cmd_curr = NULL;
    // }
	t_cmd	*begin;

	while (cmd)
	{
		begin = cmd->next;
		free(cmd->cmd);
		ft_free_split(cmd->args);
		free(cmd);
		cmd = begin;
	}
}
