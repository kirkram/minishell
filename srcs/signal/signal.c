/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:56:02 by clundber          #+#    #+#             */
/*   Updated: 2024/04/13 18:10:00 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	signal_handler(void)
{
	struct sigaction	sa1;
	struct sigaction	sa2;

	sa1.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa1, NULL);
	sa2.sa_handler = &handle_sigint;
	sa2.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa2, NULL);
}

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal = 130;
	close (STDIN_FILENO);
}
