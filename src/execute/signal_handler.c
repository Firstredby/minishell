/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aorth <aorth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:53:25 by aorth             #+#    #+#             */
/*   Updated: 2025/06/17 19:29:28 by aorth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    handle_sigs(int sig)
{
    if (sig == SIGINT)
    {
        ft_putstr_fd("\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        g_exit_status = 130; 
    }
}


void    main_sigs(void)
{
    struct sigaction sa_one;
    struct sigaction sa_two;

    sa_one.sa_handler = handle_sigs;
    sigemptyset(&sa_one.sa_mask);
    sa_one.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_one, NULL);

    sa_two.sa_handler = SIG_IGN;
    sigemptyset(&sa_two.sa_mask);
    sa_two.sa_flags = 0;
    sigaction(SIGQUIT, &sa_two, NULL);
}



void    command_sigs(void)
{
    struct sigaction sa_one;
    struct sigaction sa_two;

    sa_one.sa_handler = SIG_DFL;
    sigemptyset(&sa_one.sa_mask);
    sa_one.sa_flags = 0;
    sigaction(SIGINT, &sa_one, NULL);

    sa_two.sa_handler = SIG_DFL;
    sigemptyset(&sa_two.sa_mask);
    sa_two.sa_flags = 0;
    sigaction(SIGQUIT, &sa_two, NULL);
}