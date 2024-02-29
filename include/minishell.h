/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:17:52 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/29 16:50:14 by clundber         ###   ########.fr       */
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

typedef struct s_paths
{
	int		i;
	char	*path;
	char	*pwd;
	char	*bigpath;
	char	**paths;

}	t_paths;

typedef struct s_pipe
{
	char	**args;
	char	*cmd_with_path;
	int		*tokens;
	char	*infile;
}	t_pipe;

typedef struct s_pipeline
{
	t_pipe			**pipes;

}	t_pline;

#endif