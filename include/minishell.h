/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 17:03:41 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/13 17:04:01 by klukiano         ###   ########.fr       */
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
# include <sys/wait.h>   // for wait, waitpid, wait3, wait4
# include <stdbool.h>

# include "../lib/libft/libft.h"
# include "./parsing.h"

# define TRUE 1
# define FALSE 0

#endif
