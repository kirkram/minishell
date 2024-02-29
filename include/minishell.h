/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:17:52 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/29 10:57:26 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>      // for printf, perror
# include <stdlib.h>     // for malloc, free
# include <unistd.h>     // write, access, close, fork, execve, dup2, pipe
# include <sys/types.h>  // for wait, waitpid, wait3, wait4
# include <signal.h>     // for signal, sigaction, sigemptyset, sigaddset, kill
# include <sys/stat.h>   // for stat, lstat, fstat
# include <fcntl.h>      // for open, unlink
# include <dirent.h>     // for opendir, readdir, closedir
# include <string.h>     // for strerror
# include <termios.h>    // for tcsetattr, tcgetattr
# include <curses.h>     // for tg*** and tputs
# include <sys/ioctl.h>  // for ioctl
# include <sys/wait.h>   // for wait, waitpid, wait3, wait4

# include "../libft/include/libft.h"

typedef struct s_bigcmd
{
	char			**cmds;
	int				*tokens;
	//char			*infile;
	//char			*outfile;
	//char			*errorfile;
	struct s_bigcmd	*next;
	struct s_bigcmd	*previous;
}	t_bigcmd;
#endif
