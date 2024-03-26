/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:56:02 by clundber          #+#    #+#             */
/*   Updated: 2024/03/26 16:58:52 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	signal_handler(void)
{
	//struct sigaction sa;

/* 	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGQUIT, &sa, NULL); */
	//signal(SIGINT, handle_sigint);  //ctrt + c should quit back to promt
	//signal(SIGKILL, NULL); // ctrl + \ hould do nothing
									// ctr + D should act as eof

 	struct	sigaction sa1;


 	sa1.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa1, NULL);

	struct	sigaction sa2;

	sa2.sa_handler = &handle_sigint;
	sa2.sa_flags = SA_RESTART;
	//sigemptyset(&sa2.sa_handler);
	//sigemptyset(&sa2.sa_mask);
	sigaction(SIGINT, &sa2, NULL);
	// sa1.sa_flags = SA_RESTART;
	// sigemptyset(&sa1.sa_mask);
	// sigaction(SIGQUIT, &sa1, NULL);
}

void	handle_sigint(int sig)
{
	(void)sig;
 	int save_stdin;

	g_signal = 130;
	//ft_putendl_fd("g_signal = 130", 2);
	//write(save_stdin, '\0', 1);
	//dup2 (save_stdin, STDIN_FILENO);
	// ft_putstr_fd("\n", 1);
	// rl_on_new_line();
	//rl_redisplay();
	close (STDIN_FILENO);
	//ft_putendl_fd("", 1);
	//rl_replace_line("", 0);
	//dup2 (save_stdin, STDIN_FILENO);
	//ft_putstr_fd("minishell-0.5$ ", 1);
}

