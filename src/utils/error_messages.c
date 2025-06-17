/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 17:14:44 by ishchyro          #+#    #+#             */
/*   Updated: 2025/06/17 19:47:32 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	syn_err(t_token *token)
{
	g_exit_status = 2;
	ft_putstr_fd("syntax error near unexpected token ", 2);
	ft_putstr_fd("`", 2);
	if (token)
		ft_putstr_fd(token->token, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
}

void	miss_quote(void)
{
	g_exit_status = 2;
	ft_putstr_fd("minishell: no closing quote\n", 2);
}

void	undef_cmd(char *str)
{
	g_exit_status = 127;
	if (str)
		ft_putstr_fd(str, 2);
	ft_putstr_fd(": command not found\n", 2);
}

void	file_not_exists(char *str)
{
	g_exit_status = 1;
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}