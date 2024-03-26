/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:56:02 by clundber          #+#    #+#             */
/*   Updated: 2024/03/26 13:38:25 by clundber         ###   ########.fr       */
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
 	int fd;

	fd = dup(STDIN_FILENO);
	write(fd, NULL, 1);
	close (STDIN_FILENO);
	dup2 (fd, STDIN_FILENO);

	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	//ft_putstr_fd("minishell-0.5$ ", 1);
}

/* readline, rl_clear_history, rl_on_new_line,
rl_replace_line, rl_redisplay, add_history,
printf, malloc, free, write, access, open, read,
close, fork, wait, waitpid, wait3, wait4, signal,
sigaction, sigemptyset, sigaddset, kill, exit,
getcwd, chdir, stat, lstat, fstat, unlink, execve,
dup, dup2, pipe, opendir, readdir, closedir,
strerror, perror, isatty, ttyname, ttyslot, ioctl,
getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
tgetnum, tgetstr, tgoto, tputs */
