/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 19:22:13 by ishchyro          #+#    #+#             */
/*   Updated: 2025/05/19 20:08:43 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//202 make dynamical array of commands
//support $ sign to invoke ENV


bool	cmd_validator(char	*input, int	i)
{
	while(input[i] == ' ')
		i++;
	printf("[%s] && i == %d\n", input, i);
	if (input[i] == '\0')
		return false;
	return true;
}
int	arg_count(char **list, int pos)
{
	int	len;

	len = pos;
	while (list[len] && ft_strcmp(list[len], "|"))
		len++;
	return (len - pos);
}

t_cmd	*tokenizer(char *input, t_env *env)
{
	int	i;
	int	k;
	t_cmd *origin;
	t_cmd *cmds = NULL;
	(void) env; //i'll finish it next time
	char	**raw_input;

	i = 0;
	raw_input = ft_split(input, ' ');// split to args
	while (raw_input[i])
		i++;
	if (*raw_input[i - 1] == '|')//handling last pipe
		return (printf("error\n"), NULL);
	i = 0;
	k = 0;
	cmds = malloc(sizeof(t_cmd));
	cmds->next = NULL;
	origin = cmds;
	while (raw_input[i]) //filling cmds structs
	{
		if (!ft_strcmp(raw_input[i], "|")) // found pipe == new command
		{
			cmds->next = malloc(sizeof(t_cmd));
			cmds = cmds->next;
			cmds->next = NULL;
			i++;
		}
		cmds->cmd = ft_strdup(raw_input[i]);
		cmds->args = ft_calloc(sizeof(char *), (arg_count(raw_input, i) + 1));
		while (raw_input[i] && ft_strcmp(raw_input[i], "|") != 0)
			cmds->args[k++] = ft_strdup(raw_input[i++]);
		k = 0;
	}
	int l = 0;
	t_cmd *origin2 = origin;
	while (origin) //visualizer
	{
		printf("(cmd): %s\n", origin->cmd);
		while (origin->args[l])
		{
			printf("(arg[%d]): %s\n", l, origin->args[l]);
			l++;
		}
		l = 0;
		origin = origin->next;
	}
	ft_free_split(raw_input);
	return (origin2);
}
