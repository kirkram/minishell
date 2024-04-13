/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execve_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 17:47:29 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/13 17:53:22 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_fd_fail_pass_pipe(t_pipe **_pipe, int i, t_exec *xx)
{
	if (_pipe[1] && i != xx->num_of_pipes - 1)
		xx->tempfd_0 = dup(xx->pipefd[0]);
	close(xx->pipefd[0]);
	close(xx->pipefd[1]);
	close(xx->fd[0]);
	close(xx->fd[1]);
}

int	handle_execve_errors(char *failed_cmd)
{
	DIR	*dir;

	if (failed_cmd[0] == 0)
		return (msg_stderr("minishell: : command not found", NULL, 127));
	else if (failed_cmd[0] == '.' && failed_cmd[1] == 0)
		return (msg_stderr(".: not enough arguments", NULL, 1));
	else if (access(failed_cmd, F_OK) == -1 && ft_strchr(failed_cmd, '/'))
		return (msg_stderr(failed_cmd, ": No such file or directory", 127));
	else if (access(failed_cmd, F_OK) == -1)
		return (msg_stderr(failed_cmd, ": command not found", 127));
	else if ((access(failed_cmd, X_OK) == -1 || access(failed_cmd, R_OK) || \
	ft_strncmp(failed_cmd, "./", 3) == 0) && ft_strnstr(failed_cmd, "/", -1))
		return (msg_stderr(failed_cmd, ": Permission denied", 126));
	dir = opendir(failed_cmd);
	if (dir)
		return (handle_execve_errors_dir(failed_cmd, dir));
	else if (ft_strrchr(failed_cmd, '/') && \
	(ft_strrchr(failed_cmd, '/'))[1] == '\0')
		return (msg_stderr(failed_cmd, ": not a directory", 127));
	else
		return (msg_stderr(failed_cmd, ": command not found", 127));
	return (127);
}

int	handle_execve_errors_dir(char *failed_cmd, DIR *dir)
{
	closedir(dir);
	if (ft_strnstr(failed_cmd, "../", -1))
		return (msg_stderr \
		(ft_strnstr(failed_cmd, "../", -1), ": is a directory", 126));
	else if (ft_strnstr(failed_cmd, "./", -1))
		return (msg_stderr \
		(ft_strnstr(failed_cmd, "./", -1), ": is a directory", 126));
	else if (ft_strnstr(failed_cmd, "/", -1))
		return (msg_stderr(failed_cmd, ": is a directory", 126));
	else
		return (msg_stderr(failed_cmd, ": command not found", 127));
}
