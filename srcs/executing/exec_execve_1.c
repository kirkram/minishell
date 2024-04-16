/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execve_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 17:41:54 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/16 13:09:38 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_child_system_function(t_ms *ms, int i, t_exec *xx)
{
	t_pipe	**ppe;
	t_utils	*utils;

	ppe = ms->pipe;
	utils = ms->utils;
	xx->pid[i] = fork();
	if (xx->pid[i] == 0)
	{
		dup_and_close_child_process(i, xx);
		if ((ppe)[i]->cmd_with_path != NULL)
		{
			execve((ppe)[i]->cmd_with_path, (ppe)[i]->noio_args, utils->envp);
			utils->err_code = handle_execve_errors((ppe)[i]->cmd_with_path);
		}
		else if ((ppe)[i]->noio_args[0])
			utils->err_code = handle_execve_errors((ppe)[i]->noio_args[0]);
		free_and_exit(&ppe, &utils, ms, utils->err_code);
	}
	else
		pipe_readend_and_close_parent(i, ppe, xx);
}

void	dup_and_close_child_process(int i, t_exec *xx)
{
	if (i != 0 && xx->fd[0] < 0)
		dup2(xx->tempfd_0, STDIN_FILENO);	
	else if (xx->fd[0] > -1)
		dup2(xx->fd[0], STDIN_FILENO);
	if (i != xx->num_of_pipes - 1 && xx->fd[1] < 0)
		dup2(xx->pipefd[1], STDOUT_FILENO);
	else if (xx->fd[1] > -1)
		dup2(xx->fd[1], STDOUT_FILENO);
	close_if_valid_fd(xx->fd[0]);
	close_if_valid_fd(xx->tempfd_0);
	close_if_valid_fd(xx->pipefd[0]);
	close_if_valid_fd(xx->pipefd[1]);
	close_if_valid_fd(xx->fd[1]);
}

void	pipe_readend_and_close_parent(int i, t_pipe **_pipe, t_exec *xx)
{
	int	j;
	int	num_of_pipes;

	j = 0;
	while (_pipe[j])
		j ++;
	num_of_pipes = j;
	if (i != 0)
		close_if_valid_fd(xx->tempfd_0);
	if (_pipe[1] && i != num_of_pipes - 1)
		xx->tempfd_0 = dup(xx->pipefd[0]);
	close_if_valid_fd(xx->pipefd[0]);
	close_if_valid_fd(xx->pipefd[1]);
	close_if_valid_fd(xx->fd[0]);
	close_if_valid_fd(xx->fd[1]);
}
