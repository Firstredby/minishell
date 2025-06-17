/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboxuser <vboxuser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 17:14:44 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/16 20:18:04 by vboxuser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	syn_err(t_token *token)
{
	ft_putstr_fd("syntax error near unexpected token ", 2);
	ft_putstr_fd("`", 2);
	if (token)
		ft_putstr_fd(token->token, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	g_exit_status = 2;
}

void	miss_quote(void)
{
	ft_putstr_fd("minishell: no closing quote\n", 2);
	g_exit_status = 2;
}

void	undef_cmd(char *str)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": command not found\n", 2);
	g_exit_status = 127;
}

void	file_not_exists(char *str)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	g_exit_status = 1;
}