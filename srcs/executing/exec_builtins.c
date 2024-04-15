/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 17:50:51 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/15 18:44:44 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_child_builtin_function(t_ms *ms, int i, t_exec *xx)
{
	xx->pid[i] = fork();
	if (xx->pid[i] == 0)
	{
		dup_and_close_child_process(i, xx);
		ms->utils->err_code = exec_builtin(ms->pipe, ms->utils, i, ms);
		free_and_exit(&ms->pipe, &ms->utils, ms, ms->utils->err_code);
	}
	else
		pipe_readend_and_close_parent(i, ms->pipe, xx);
}

void	exec_builtin_no_pipes(t_ms *ms, int i, t_exec *xx)
{
	if (xx->fd[0] > -1)
		dup2(xx->fd[0], STDIN_FILENO);
	close_if_valid_fd(xx->fd[0]);
	if (xx->fd[1] > -1)
		dup2(xx->fd[1], STDOUT_FILENO);
	close_if_valid_fd(xx->fd[1]);
	ms->utils->err_code = exec_builtin(ms->pipe, ms->utils, i, ms);
}

int	exec_builtin(t_pipe **_pipe, t_utils *utils, int i, t_ms *ms)
{
	if (!ft_strncmp((_pipe)[i]->noio_args[0], "echo", -1))
		return (echo_builtin((_pipe)[i]->noio_args, utils));
	else if (!ft_strncmp((_pipe)[i]->noio_args[0], "cd", -1))
		return (cd_builtin(_pipe, utils, i, ms));
	else if (!ft_strncmp((_pipe)[i]->noio_args[0], "pwd", -1))
		return (pwd(utils));
	else if (!ft_strncmp((_pipe)[i]->noio_args[0], "export", -1))
	{
		if (_pipe[0] && !_pipe[1])
			return (export(utils, (_pipe)[i]->noio_args, ms));
		else
			return (0);
	}
	else if (!ft_strncmp((_pipe)[i]->noio_args[0], "unset", -1))
		return (unset(utils, (_pipe)[i]->noio_args, ms));
	else if (!ft_strncmp((_pipe)[i]->noio_args[0], "env", -1))
		return (env(utils, (_pipe)[i]->noio_args));
	else if (!ft_strncmp((_pipe)[i]->noio_args[0], "exit", -1))
		return (exit_builtin(_pipe, utils, i, ms));
	return (42);
}
