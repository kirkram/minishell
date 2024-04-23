/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execve_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 17:47:29 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/23 15:01:28 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_fd_fail_pass_pipe(t_pipe **_pipe, int i, t_exec *xx, t_ms *ms)
{
	if (_pipe[1] && i != xx->num_of_pipes - 1)
		xx->tempfd_0 = dup_and_check(xx->pipefd[0], ms);
	close_if_valid_fd(xx->pipefd[0]);
	close_if_valid_fd(xx->pipefd[1]);
	close_if_valid_fd(xx->fd[0]);
	close_if_valid_fd(xx->fd[1]);
}

static int	handle_execve_errors_dir(char *failed_cmd)
{
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

int	handle_execve_errors(char *failed_cmd, int found_path, t_ms *ms, int i)
{
	if (access(failed_cmd, F_OK) == -1 && ft_strchr(failed_cmd, '/'))
		return (msg_stderr(failed_cmd, ": No such file or directory", 127));
	else if (check_is_dir(failed_cmd))
		return (handle_execve_errors_dir(failed_cmd));
	else if (access(failed_cmd, F_OK) == -1 || \
	(!ft_strchr(failed_cmd, '/') && !found_path))
		return (msg_stderr(failed_cmd, ": command not found", 127));
	else if (access(failed_cmd, X_OK) == -1)
	{
		if (!ft_strncmp(ms->pipe[i]->noio_args[0], "./", 2) || \
			!ft_strncmp(ms->pipe[i]->noio_args[0], "../", 3))
			return (msg_stderr(ms->pipe[i]->noio_args[0], \
			": Permission denied", 126));
		return (msg_stderr(failed_cmd, ": Permission denied", 126));
	}
	else if (ft_strrchr(failed_cmd, '/') && \
	(ft_strrchr(failed_cmd, '/'))[1] == '\0')
		return (msg_stderr(failed_cmd, ": not a directory", 127));
	else
		return (msg_stderr(failed_cmd, ": command not found", 127));
	return (127);
}

int	check_is_dir(char *path)
{
	struct stat	buf;
	bool		is_dir;

	is_dir = false;
	if (stat(path, &buf) != -1)
		if (!S_ISREG(buf.st_mode))
			is_dir = true;
	return (is_dir);
}
