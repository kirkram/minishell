/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 17:58:15 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/19 12:47:19 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//OPEN INFILE, OPEN/CREATE OUTFILE AND TRY TO ACCESS
int	exec_assign_redirections(t_pipe *_pipe_i, int (*fd)[2])
{
	int	fd_failed;
	int	*tokens;
	int	j;

	tokens = _pipe_i->tokens;
	fd_failed = 0;
	j = 0;
	while (_pipe_i->args[j] && tokens[j] != 0)
	{
		if (tokens[j] == SKIP_IN || tokens[j] == IN_FD || tokens[j] == IN_HD)
		{
			if (exec_redir_in(_pipe_i, fd, j, &fd_failed) == 1)
				break ;
		}
		else if (tokens[j] == OUT || tokens[j] == OUT_AP || \
		tokens[j] == SKIP_OUT)
		{
			if (exec_redir_out(_pipe_i, fd, j, &fd_failed) == 1)
				break ;
		}
		j ++;
	}
	return (fd_failed);
}

static int	exec_redir_out_err(char *failed_fd, char *msg, int *fd_failed)
{
	ft_putstr_fd("minishell: ", 2);
	if (failed_fd)
		ft_putstr_fd(failed_fd, 2);
	ft_putendl_fd(msg, 2);
	*fd_failed = 1;
	return (1);
}

int	exec_redir_out(t_pipe *_pipe_i, int (*fd)[2], int j, int *fd_failed)
{
	if (_pipe_i->tokens[j] == SKIP_OUT)
	{
		(*fd)[1] = open(_pipe_i->args[j], O_CREAT | O_WRONLY, 0644);
		close_if_valid_fd((*fd)[1]);
	}
	else if (_pipe_i->tokens[j] == OUT)
		(*fd)[1] = open(_pipe_i->args[j], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (_pipe_i->tokens[j] == OUT_AP)
		(*fd)[1] = open(_pipe_i->args[j], O_CREAT | O_WRONLY | O_APPEND, 0644);
	if ((*fd)[1] < 0 && (_pipe_i->args[j][0] == '\0' || \
		access(_pipe_i->args[j], F_OK) == -1))
		return (exec_redir_out_err(_pipe_i->args[j], \
		": No such file or directory", fd_failed));
	else if ((*fd)[1] < 0 && check_is_dir(_pipe_i->args[j]))
		return (exec_redir_out_err(_pipe_i->args[j], \
		": Is a directory", fd_failed));
	else if ((*fd)[1] < 0)
		return (exec_redir_out_err(_pipe_i->args[j], \
		": Permission denied", fd_failed));
	return (0);
}

int	exec_redir_in(t_pipe *_pipe_i, int (*fd)[2], int j, int *fd_failed)
{
	if (_pipe_i->tokens[j] == IN_FD)
		(*fd)[0] = open (_pipe_i->args[j], O_RDONLY);
	else if (_pipe_i->tokens[j] == IN_HD)
		(*fd)[0] = _pipe_i->hd_fd[0];
	if (_pipe_i->tokens[j] != IN_HD && \
	(access(_pipe_i->args[j], F_OK) == -1 || \
	access(_pipe_i->args[j], R_OK) == -1))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(_pipe_i->args[j], 2);
		if (access(_pipe_i->args[j], F_OK) == -1)
			ft_putendl_fd(": No such file or directory", 2);
		else if (access(_pipe_i->args[j], R_OK) == -1)
			ft_putendl_fd(": Permission denied", 2);
		*fd_failed = 1;
		return (1);
	}
	return (0);
}
