/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:56:02 by clundber          #+#    #+#             */
/*   Updated: 2024/04/17 18:18:40 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	signal_handler(void)
{
	struct sigaction	sa2;

	sa2.sa_handler = &handle_sigint;
	sa2.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa2, NULL);
}

void	handle_sigint(int sig)
{
	g_signal = sig;
	if (sig == 3)
		ft_putstr_fd("Quit: 3", 2);
	close (STDIN_FILENO);
}

void	handle_sigquit(bool ignored)
{
	struct sigaction	sa1;

	if (ignored == true)
		sa1.sa_handler = SIG_IGN;
	else
	{
		sa1.sa_handler = SIG_DFL;
		sa1.sa_handler = &handle_sigint;
	}
	sigaction(SIGQUIT, &sa1, NULL);
}
