/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:59:27 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/10 18:12:44 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*assign_scmd_path(char *scmd, char **envp)
{
	char	**env_paths;
	char	*cmd_path;
	int		i;

	//should move it function up so that it doesnt waste resources every time
	if (!scmd || scmd[0] == '\0')
		return (NULL);
	env_paths = find_path_and_pwd(envp, scmd);
	if (!env_paths)
		return (NULL);
	i = 0;
	while (env_paths[i])
	{
		if (scmd && scmd[0] == '/')
		{
			cmd_path = ft_strdup(scmd);
			if (!cmd_path)
				malloc_error(1);
			free_and_1(env_paths, NULL);
			if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) == 0)
				return (cmd_path);
			else
				return (NULL);
		}
		else
			cmd_path = jointhree(env_paths[i], "/", scmd);
		if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) == 0)
		{
			free_and_1(env_paths, NULL);
			return (cmd_path);
		}
		free (cmd_path);
		i ++;
	}
	free_and_1(env_paths, NULL);
	return (NULL);
}

char	**find_path_and_pwd(char **envp, char *scmd)
{
	t_paths	vars;

	ft_bzero(&vars, sizeof(t_paths));
	if (!ft_strnstr(scmd, "./", -1))
		vars.should_skip_pwd = true;
	while (envp[vars.i])
	{
		if (ft_strncmp(envp[vars.i], "PATH=", 5) == 0)
			vars.path = envp[vars.i] + 5;
		else if (ft_strncmp(envp[vars.i], "PWD=", 4) == 0)
			vars.pwd = envp[vars.i] + 4;
		vars.i ++;
	}
	vars.bigpath = jointhree(vars.path, ":", vars.pwd);
	if (vars.bigpath && !vars.should_skip_pwd)
		vars.paths = ft_split(vars.bigpath, ':');
	else if (vars.path)
		vars.paths = ft_split(vars.path, ':');
	else if (!vars.should_skip_pwd)
		vars.paths = ft_split(vars.pwd, ':');
	if(((vars.bigpath && !vars.should_skip_pwd) || (!vars.bigpath && vars.path) || \
	(!vars.path && !vars.should_skip_pwd)) && !vars.paths)
		malloc_error(1);
	free (vars.bigpath);
	return (vars.paths);
}

int	exec_redirections_out(t_pipe *_pipe_i, int (*fd)[2], int j, int *has_fd_failed)
{
	if (_pipe_i->tokens[j] == SKIP_OUT)
	{
		(*fd)[1] = open(_pipe_i->args[j], O_CREAT | O_WRONLY, 0644);
		close ((*fd)[1]);
	}
	else if (_pipe_i->tokens[j] == OUT)
		(*fd)[1] = open(_pipe_i->args[j], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (_pipe_i->tokens[j] == OUT_AP)
		(*fd)[1] = open(_pipe_i->args[j], O_CREAT | O_WRONLY | O_APPEND, 0644);
	if ((*fd)[1] < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(_pipe_i->args[j], 2);
		ft_putendl_fd(": Permission denied", 2);
		*has_fd_failed = 1;
		return (1);
	}
	return (0);
}
int	exec_redirections_in(t_pipe *_pipe_i, int (*fd)[2], int j, int *has_fd_failed)
{
	if (_pipe_i->tokens[j] == IN_FD)
		(*fd)[0] = open (_pipe_i->args[j], O_RDONLY);
	else if (_pipe_i->tokens[j] == IN_HD)
		(*fd)[0] = _pipe_i->hd_fd[0];
	if (_pipe_i->tokens[j] != IN_HD && \
	(access(_pipe_i->args[j], F_OK) == -1 || access(_pipe_i->args[j], R_OK) == -1))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(_pipe_i->args[j], 2);
		if (access(_pipe_i->args[j], F_OK) == -1)
			ft_putendl_fd(": No such file or directory", 2);
		else if (access(_pipe_i->args[j], R_OK) == -1)
			ft_putendl_fd(": Permission denied", 2);
		*has_fd_failed = 1;
		return (1);
	}
	return (0);
}

//OPEN INFILE, OPEN/CREATE OUTFILE AND TRY TO ACCESS
int	exec_assign_redirections(t_pipe *_pipe_i, int (*fd)[2])
{
	int	has_fd_failed;
	int	*tokens;
	int	j;

	tokens = _pipe_i->tokens;
	has_fd_failed = 0;
	j = 0;
	while(_pipe_i->args[j] && tokens[j] != 0)
	{
		if (tokens[j] == SKIP_IN || tokens[j] == IN_FD || tokens[j] == IN_HD)
		{
			if (exec_redirections_in(_pipe_i, fd, j, &has_fd_failed) == 1)
				break ;
		}
		else if (tokens[j] == SKIP_OUT || tokens[j] == OUT || tokens[j] == OUT_AP)
		{
			if (exec_redirections_out(_pipe_i, fd, j, &has_fd_failed) == 1)
				break ;
		}
		j ++;
	}
	return (has_fd_failed);
}

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
	}
	if (*_pipe && (*_pipe)[i])
		free((*_pipe)[i]);
	(*_pipe) = NULL;
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
		xx.has_fd_failed = 0;
		xx.has_fd_failed = exec_assign_redirections(ms->pipe[i], &xx.fd);
		if (xx.has_fd_failed)
		{
			close(xx.fd[0]);
			close(xx.fd[1]);
			xx.fd[0] = -1;
			xx.fd[1] = -2;
			utils->err_code = 1;
		}
		if (i != xx.num_of_pipes - 1)
			pipe(xx.pipefd);
		if (!xx.has_fd_failed && _pipe[i]->tokens && _pipe[i]->tokens[0] != BUILTIN)
			exec_child_system_function(_pipe, utils, i, &xx);
		else if (!xx.has_fd_failed && _pipe[1] && _pipe[i]->tokens[0] == BUILTIN)
			exec_child_builtin_function(ms, i, &xx);
		else if (!xx.has_fd_failed && !_pipe[1])
			exec_builtin_no_pipes(ms, i, &xx);
		else if (xx.has_fd_failed)
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

int	waitpid_and_close_exec(t_ms *ms, t_exec *xx)
{
	int		i;
	int		child_exit_code;

	i = 0;
	child_exit_code = 0;
	if (ms->pipe[i]->tokens && ms->pipe[i]->tokens[0] == BUILTIN && !ms->pipe[1])
			i ++;
	while (i < xx->num_of_pipes)
	{
		if (i == xx->num_of_pipes - 1)
			waitpid(xx->pid[i], &child_exit_code, 0);
		else
			waitpid(xx->pid[i], NULL, 0);
		i ++;
	}
	dup2(xx->savestdio[0], STDIN_FILENO);
	dup2(xx->savestdio[1], STDOUT_FILENO);
	close(xx->savestdio[0]);
	close(xx->savestdio[1]);
	if (g_signal == 130)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (130);
	}
	if (WIFEXITED(child_exit_code) && !xx->has_fd_failed && \
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
			return (unset(utils, (_pipe)[i]->noio_args));
	else if (!ft_strncmp((_pipe)[i]->noio_args[0], "env", -1))
		return (env(utils));
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

int	msg_stderr(char *cmd, char *message, int err_code)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
		ft_putstr_fd(cmd, 2);
	if (message)
		ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	return (err_code);
}

char	*jointhree(char const *s1, char const *s2, char const *s3)
{
	char	*newstr;

	if (s1 && s2 && s3)
	{
		newstr = malloc(((ft_strlen(s1) + ft_strlen(s2) \
		+ ft_strlen(s3)) + 1) * sizeof(char));
		if (newstr == NULL)
			malloc_error(1);
		ft_strlcpy(newstr, (char *)s1, -1);
		ft_strlcpy(newstr + ft_strlen(newstr), (char *)s2, -1);
		ft_strlcpy(newstr + ft_strlen(newstr), (char *)s3, -1);
		return (newstr);
	}
	return (NULL);
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
int	free_and_1(char **paths, int **end)
{
	int	i;

	i = 0;
	if (paths && *paths)
	{
		while (paths[i])
		{
			free (paths[i]);
			i ++;
		}
		*paths = NULL;
		free (paths);
	}
	if (end)
	{
		free (*end);
		(*end) = NULL;
	}
	return (1);
}
