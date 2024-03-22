/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:56:02 by clundber          #+#    #+#             */
/*   Updated: 2024/03/22 18:18:59 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	signal_handler(void)
{
	//signal(SIGINT, handle_sigint);  //ctrt + c should quit back to promt
	//signal(SIGKILL, NULL); // ctrl + \ hould do nothing
									// ctr + D should act as eof

	struct	sigaction sa1;
	// struct	sigaction sa2;


	// sa1.sa_flags = SA_RESTART;
	sa1.sa_handler = &handle_sigquit;
	sigaction(SIGQUIT, &sa1, NULL);
	// sigemptyset(&sa1.sa_mask);
	// sigaction(SIGQUIT, &sa1, NULL);

	// sa2.sa_handler = &handle_sigint;
	// sa2.sa_flags = SA_RESTART;
	// sigemptyset(&sa2.sa_mask);
	// sigaction(SIGINT, &sa2, NULL);
}

void	handle_sigint(int sig)
{
	(void)sig;
	ft_putendl_fd("hehe", 1);
	// ft_putstr_fd("minishell-0.5$ ", 1);
}

void	handle_sigquit(int sig)
{
	ft_putendl_fd("ctl dash ", 1);
	(void)sig;
}
