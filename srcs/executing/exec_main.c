/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:59:27 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/13 15:58:30 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void	dup_and_close_child_process(int i, t_exec *xx)
{
	if (i != 0 && xx->fd[0] < 0 && dup2(xx->tempfd_0, STDIN_FILENO) == -1)
		exit(42);
	else if (xx->fd[0] > -1 && dup2(xx->fd[0], STDIN_FILENO) == -1)
		exit(42);
	if (i != xx->num_of_pipes - 1 && xx->fd[1] < 0 && \
	dup2(xx->pipefd[1], STDOUT_FILENO) == -1)
		exit(42);
	else if (xx->fd[1] > -1 && dup2(xx->fd[1], STDOUT_FILENO) == -1)
		exit(42);
	close (xx->fd[0]);
	close (xx->tempfd_0);
	close (xx->pipefd[0]);
	close (xx->pipefd[1]);
	close (xx->fd[1]);
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
		close(xx->tempfd_0);
	if (_pipe[1] && i != num_of_pipes - 1)
		xx->tempfd_0 = dup(xx->pipefd[0]);
	close(xx->pipefd[0]);
	close(xx->pipefd[1]);
	close(xx->fd[0]);
	close(xx->fd[1]);
}

//if exit code is -42 then it doesn't exit
void	free_pipes_utils_and_exit(t_pipe ***_pipe, t_utils **utils, int child_exit_code)
{
	int i;

	i = 0;
	if (_pipe && *_pipe)
	{
		while ((*_pipe)[i])
		{
			ft_arrfree((*_pipe)[i]->args);
			ft_arrfree((*_pipe)[i]->noio_args);
			free((*_pipe)[i]->tokens);
			free((*_pipe)[i]->cmd_with_path);
			free((*_pipe)[i]);
			i ++;
		}
		free((*_pipe)[i]);
		free(*_pipe);
		(*_pipe) = NULL;
	}
	if (utils && *utils)
	{
		ft_arrfree((*utils)->envp);
		ft_arrfree((*utils)->export);
	}
	if (child_exit_code != -42)
		exit (child_exit_code);
}

void	exec_child_system_function(t_pipe **_pipe, t_utils *utils, int i, t_exec *xx)
{
	xx->pid[i] = fork();
	if (xx->pid[i] == 0)
	{
		dup_and_close_child_process(i, xx);
		if ((_pipe)[i]->cmd_with_path != NULL)
		{
			execve((_pipe)[i]->cmd_with_path, (_pipe)[i]->noio_args, utils->envp);
			utils->err_code = handle_execve_errors((_pipe)[i]->cmd_with_path);
		}
		else if ((_pipe)[i]->noio_args[0])
			utils->err_code = handle_execve_errors((_pipe)[i]->noio_args[0]);
		free_pipes_utils_and_exit(&_pipe, &utils, utils->err_code);
	}
	else
		pipe_readend_and_close_parent(i, _pipe, xx);
}

void	exec_child_builtin_function(t_ms *ms, int i, t_exec *xx)
{
	xx->pid[i] = fork();
	if (xx->pid[i] == 0)
	{
		dup_and_close_child_process(i, xx);
		ms->utils->err_code = exec_builtin(ms->pipe, ms->utils, i, ms);
		free_pipes_utils_and_exit(&ms->pipe, &ms->utils, ms->utils->err_code);
	}
	else
		pipe_readend_and_close_parent(i, ms->pipe, xx);
}

void exec_builtin_no_pipes(t_ms *ms, int i, t_exec *xx)
{
	if (xx->fd[0] > -1)
		dup2(xx->fd[0], STDIN_FILENO);
	close(xx->fd[0]);
	if (xx->fd[1] > -1)
		dup2(xx->fd[1], STDOUT_FILENO);
	close(xx->fd[1]);
	ms->utils->err_code = exec_builtin(ms->pipe, ms->utils, i, ms);
}

void	exec_fd_fail_pass_pipe(t_pipe **_pipe, int i, t_exec *xx)
{
	if (_pipe[1] && i != xx->num_of_pipes - 1)
		xx->tempfd_0 = dup(xx->pipefd[0]);
	close(xx->pipefd[0]);
	close(xx->pipefd[1]);
	close(xx->fd[0]);
	close(xx->fd[1]);
}

int	execute(t_utils *utils, t_pipe **_pipe, t_ms *ms)
{
	t_exec	xx;
	int		i;

	if (g_signal == 130)
		return (130);
	xx.savestdio[0] = dup(STDIN_FILENO);
	xx.savestdio[1] = dup(STDOUT_FILENO);
	i = 0;
	while (ms->pipe[i])
		i ++;
	xx.num_of_pipes = i;
	xx.tempfd_0 = -1;
	xx.pipefd[0] = -1;
	xx.pipefd[1] = -2;
	i = 0;
	while (i < xx.num_of_pipes && i < 256)
	{
		if (g_signal == 130)
			break;
		xx.fd[0] = -1;
		xx.fd[1] = -2;
		xx.fd_failed = 0;
		xx.fd_failed = exec_assign_redirections(ms->pipe[i], &xx.fd);
		if (xx.fd_failed)
		{
			close(xx.fd[0]);
			close(xx.fd[1]);
			xx.fd[0] = -1;
			xx.fd[1] = -2;
			utils->err_code = 1;
		}
		if (i != xx.num_of_pipes - 1)
			pipe(xx.pipefd);
		if (!xx.fd_failed && _pipe[i]->tokens && _pipe[i]->tokens[0] != BUILTIN)
			exec_child_system_function(_pipe, utils, i, &xx);
		else if (!xx.fd_failed && _pipe[1] && _pipe[i]->tokens[0] == BUILTIN)
			exec_child_builtin_function(ms, i, &xx);
		else if (!xx.fd_failed && !_pipe[1])
			exec_builtin_no_pipes(ms, i, &xx);
		else if (xx.fd_failed)
			exec_fd_fail_pass_pipe(_pipe, i, &xx);
		i ++;
	}
	if (i == 256)
	{
		ft_putendl_fd("Unsupported number of pipes, exiting ...", 2);
		exit (42);
	}
	close(xx.tempfd_0);
	return (waitpid_and_close_exec(ms, &xx));
}

//return exit code of last child if there was no fd failure and there was a pipe in the command
int	waitpid_and_close_exec(t_ms *ms, t_exec *xx)
{
	int		child_exit_code;

	xx->i = 0;
	child_exit_code = 0;
	if (ms->pipe[xx->i]->tokens && ms->pipe[xx->i]->tokens[0] == BUILTIN && !ms->pipe[1])
			xx->i ++;
	while (xx->i < xx->num_of_pipes)
	{
		if (xx->i == xx->num_of_pipes - 1)
			waitpid(xx->pid[xx->i], &child_exit_code, 0);
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
	else if (WIFEXITED(child_exit_code) && !xx->fd_failed && \
	!(ms->pipe[0]->tokens[0] == BUILTIN && !ms->pipe[1]))
		return (WEXITSTATUS(child_exit_code));
	else
		return (ms->utils->err_code);
}

int		exec_builtin(t_pipe **_pipe, t_utils *utils, int i, t_ms *ms)
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
		return (exit_builtin(_pipe, utils, i));
	return (42);
}

int		handle_execve_errors(char *failed_cmd)
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
	else if ((access(failed_cmd, X_OK) == -1 || access(failed_cmd, R_OK) == -1 || \
	ft_strncmp(failed_cmd, "./", 3) == 0) && ft_strnstr(failed_cmd, "/", -1))
		return (msg_stderr(failed_cmd, ": Permission denied", 126));
	dir = opendir(failed_cmd);
	if (dir)
	{
		closedir(dir);
		if (ft_strnstr(failed_cmd, "../", -1))
			return (msg_stderr(ft_strnstr(failed_cmd, "../", -1), ": is a directory", 126));
		else if (ft_strnstr(failed_cmd, "./", -1))
			return (msg_stderr(ft_strnstr(failed_cmd, "./", -1), ": is a directory", 126));
		else if (ft_strnstr(failed_cmd, "/", -1))
			return (msg_stderr(failed_cmd, ": is a directory", 126));
		else
			return (msg_stderr(failed_cmd, ": command not found", 127));
	}
	else if (ft_strrchr(failed_cmd, '/') && (ft_strrchr(failed_cmd, '/'))[1] == '\0')
			return (msg_stderr(failed_cmd, ": not a directory", 127));
	else
		return (msg_stderr(failed_cmd, ": command not found", 127));
	return (127);
}

int	user_cmd_path(char **args, char *arg_cmd, char **paths)
{
	int	i;

	i = 0;
	arg_cmd += 2;
	if (args[0] && args[1])
		return (free_and_1(args, NULL));
	free (args[0]);
	args[0] = malloc(ft_strlen(arg_cmd) + 1);
	if (!args[0])
		malloc_error(1);
	while (arg_cmd[i])
	{
		args[0][i] = arg_cmd[i];
		i ++;
	}
	args[0][i] = '\0';
	i = 0;
	while (paths[i])
		i ++;
	i --;
	return (i);
}

void	delete_pwd_path(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		i ++;
	i --;
	free (paths[i]);
	paths[i] = NULL;
}

