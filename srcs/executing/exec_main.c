/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:59:27 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/13 18:08:26 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute(t_utils *utils, t_pipe **_pipe, t_ms *ms)
{
	t_exec	xx;

	if (g_signal == 130)
		return (130);
	xx.savestdio[0] = dup(STDIN_FILENO);
	xx.savestdio[1] = dup(STDOUT_FILENO);
	xx.i = 0;
	while (ms->pipe[xx.i])
		xx.i ++;
	xx.num_of_pipes = xx.i;
	xx.tempfd_0 = -1;
	xx.pipefd[0] = -1;
	xx.pipefd[1] = -2;
	xx.i = 0;
	while (xx.i < xx.num_of_pipes && xx.i < 256)
	{
		if (execute_loop(utils, _pipe, ms, &xx) == 1)
			break ;
		xx.i ++;
	}
	if (xx.i == 256)
		exit (42);
	close(xx.tempfd_0);
	return (waitpid_and_close_exec(ms, &xx));
}

int	execute_loop(t_utils *utils, t_pipe **_pipe, t_ms *ms, t_exec *xx)
{
	if (g_signal == 130)
		return (1);
	xx->fd[0] = -1;
	xx->fd[1] = -2;
	xx->fd_failed = 0;
	xx->fd_failed = exec_assign_redirections(ms->pipe[xx->i], &xx->fd);
	if (xx->fd_failed)
	{
		close(xx->fd[0]);
		close(xx->fd[1]);
		xx->fd[0] = -1;
		xx->fd[1] = -2;
		utils->err_code = 1;
	}
	if (xx->i != xx->num_of_pipes - 1)
		pipe(xx->pipefd);
	if (!xx->fd_failed && _pipe[xx->i]->tokens && _pipe[xx->i]->tokens[0] != 8)
		exec_child_system_function(ms, xx->i, xx);
	else if (!xx->fd_failed && _pipe[1] && _pipe[xx->i]->tokens[0] == BUILTIN)
		exec_child_builtin_function(ms, xx->i, xx);
	else if (!xx->fd_failed && !_pipe[1])
		exec_builtin_no_pipes(ms, xx->i, xx);
	else if (xx->fd_failed)
		exec_fd_fail_pass_pipe(_pipe, xx->i, xx);
	return (0);
}

//return exit code of last child
//if there was no fd failure and there was a pipe in the command
int	waitpid_and_close_exec(t_ms *ms, t_exec *xx)
{
	xx->i = 0;
	xx->child_exit_code = 0;
	if (ms->pipe[xx->i]->tokens && ms->pipe[xx->i]->tokens[0] == BUILTIN && \
	!ms->pipe[1])
		xx->i ++;
	while (xx->i < xx->num_of_pipes)
	{
		if (xx->i == xx->num_of_pipes - 1)
			waitpid(xx->pid[xx->i], &xx->child_exit_code, 0);
		else
			waitpid(xx->pid[xx->i], NULL, 0);
		xx->i ++;
	}
	dup2(xx->savestdio[0], STDIN_FILENO);
	dup2(xx->savestdio[1], STDOUT_FILENO);
	close(xx->savestdio[0]);
	close(xx->savestdio[1]);
	if (g_signal == 130 && ft_putstr_fd("\n", STDOUT_FILENO) != -42)
		return (130);
	else if (WIFEXITED(xx->child_exit_code) && !xx->fd_failed && \
	!(ms->pipe[0]->tokens[0] == BUILTIN && !ms->pipe[1]))
		return (WEXITSTATUS(xx->child_exit_code));
	else
		return (ms->utils->err_code);
}
