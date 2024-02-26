/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:17:52 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/26 18:37:44 by klukiano         ###   ########.fr       */
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

typedef struct s_cmd
{
	char			**cmd;
	//char			**flags;
	int				pipe[2];
	int				num_of_cmds;
	char			*infile;
	char			*outfile;
	char			*errorfile;
	char			**paths;
	struct s_cmd	*next;
	struct s_cmd	*previous;s
	// int				entry_pipe;
	// int				exit_pipe;
}	t_cmd;
#endif
