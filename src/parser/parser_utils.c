/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishchyro <ishchyro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:44:46 by codespace         #+#    #+#             */
/*   Updated: 2025/06/11 21:26:40 by ishchyro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*replace_string(char *str, char *var, int i, int k)
{
	char	*newstr;
	int		size;
	int		str_pos;
	int		var_pos;

	str_pos = 0;
	var_pos = 0;
	size = i - 1 + ft_strlen(var) + (ft_strlen(str) - i - k + 1);
	newstr = ft_calloc(size, sizeof(char));
	if (!newstr)
		return (NULL);//err
	while (str_pos < i - 1)
	{
		newstr[str_pos] = str[str_pos];
		str_pos++;
	}
	while (var && var[var_pos])
		newstr[str_pos++] = var[var_pos++];
	while (str[i + k])
		newstr[str_pos++] = str[i + k++];
	free(str);
	free(var);
	str = newstr;
	return (newstr);
}

char    *env_from_list(t_env *env, char *key)
{
	t_env *head;

	head = env;
	if (!ft_strcmp(key, "?"))
	{
		free(key);
		key = ft_itoa(0);
		return (key);
	}
	while (head)
	{
		if (!ft_strcmp(head->key, key))
		{
			free(key);
			key = ft_strdup(head->value);
			return (key);
		}
		head = head->next;
	}
	free(key);
	key = ft_calloc(1, 1);
	return (key);
}

void	open_fd(t_token *token, t_cmd *cmd, int redir)
{
    int flags;
	int *fd;

    if (redir == T_RED_IN)
	{
		flags = O_RDONLY;
		fd = &cmd->fd_in;
	}
    else
		fd = &cmd->fd_out;
	if (redir == T_RED_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
    if (*fd)
		close(*fd);
    *fd = open(token->next->token, flags, 0644);
}