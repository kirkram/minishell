/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:59:27 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/06 14:47:11 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//test in the file change

#include "../../include/minishell.h"
extern int g_signal;

char	*assign_scmd_path(char *scmd, char **envp)
{
	char	**env_paths;
	char	*cmd_path;
	int		i;

	//should move it function up so that it doesnt waste resources every time
	if (!scmd)
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

	vars.paths = NULL;
	vars.i = 0;
	vars.path = NULL;
	vars.pwd = NULL;
	vars.should_skip_pwd = false;
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
	{
		vars.paths = ft_split(vars.bigpath, ':');
		if (!vars.paths)
			malloc_error(1);
	}
	else if (vars.path)
	{
		vars.paths = ft_split(vars.path, ':');
		if (!vars.paths)
			malloc_error(1);
	}
	else if (!vars.should_skip_pwd)
	{
		vars.paths = ft_split(vars.pwd, ':');
		if (!vars.paths)
			malloc_error(1);
	}
	free (vars.bigpath);
	return (vars.paths);
}

//OPEN INFILE, OPEN/CREATE OUTFILE AND TRY TO ACCESS
int	exec_assign_redirections(t_pipe *_pipe_i, int (*fd)[2], char **infile, char **outfile)
{
	int	has_fd_failed;
	int	*tokens;
	int	j;

	tokens = _pipe_i->tokens;
	has_fd_failed = 0;
	j = 0;
	while(_pipe_i->args[j] && tokens[j] != 0)
	{
		if (_pipe_i->tokens[j] == SKIP_IN || _pipe_i->tokens[j] == IN_FD || _pipe_i->tokens[j] == IN_HD)
		{
			if (_pipe_i->tokens[j] == IN_FD)
			{
				(*fd)[0] = open (_pipe_i->args[j], O_RDONLY);
				*infile = _pipe_i->args[j];
			}
			else if (_pipe_i->tokens[j] == IN_HD)
			{
				(*fd)[0] = _pipe_i->hd_fd[0];
				*infile = _pipe_i->args[j];
			}
			if (_pipe_i->tokens[j] != IN_HD && \
			(access(_pipe_i->args[j], F_OK) == -1 || access(_pipe_i->args[j], R_OK) == -1))
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(_pipe_i->args[j], 2);
				if (access(_pipe_i->args[j], F_OK) == -1)
					ft_putendl_fd(": No such file or directory", 2);
				else if (access(_pipe_i->args[j], R_OK) == -1)
					ft_putendl_fd(": Permission denied", 2);
				has_fd_failed = 1;
				break ;
			}
		}
		else if (tokens[j] == SKIP_OUT || tokens[j] == OUT || tokens[j] == OUT_AP)
		{
			if (tokens[j] == SKIP_OUT)
				(*fd)[1] = open(_pipe_i->args[j], O_CREAT | O_WRONLY, 0644);
			else if (tokens[j] == OUT)
				(*fd)[1] = open(_pipe_i->args[j], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			else if (tokens[j] == OUT_AP)
				(*fd)[1] = open(_pipe_i->args[j], O_CREAT | O_WRONLY | O_APPEND, 0644);
			if ((*fd)[1] < 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(_pipe_i->args[j], 2);
				ft_putendl_fd(": Permission denied", 2);
				has_fd_failed = 1;
				break ;
			}
			else if (tokens[j] != SKIP_OUT)
				*outfile = _pipe_i->args[j];
		}
		else if (tokens[j] == 0)
			ft_putendl_fd("UNEQUAL TOKENS AND ARGS COUNT", 2);
		j ++;
	}
	return (has_fd_failed);
}

//if exit code is -42 doesn't exit
void	free_pipes_utils_and_exit(t_pipe **_pipe, t_utils *utils, int child_exit_code)
{
	int i;

	i = 0;
	while (_pipe[i])
	{
		ft_arrfree(_pipe[i]->args);
		ft_arrfree(_pipe[i]->noio_args);
		free(_pipe[i]->tokens);
		free(_pipe[i]->cmd_with_path);
		free(_pipe[i]);
		i ++;
	}
	free (_pipe);
	if (utils)
	{
		ft_arrfree(utils->envp);
		ft_arrfree(utils->export);
		free(utils);
	}
	if (child_exit_code != -42)
		exit (child_exit_code);
}

int	execute(t_utils *utils, t_pipe **_pipe)
{
	//int			fd[2];
	//int			savestdio[2];
	pid_t		pid[256]; //zsh limit descriptors = 256
	int			pipefd[2];
	int			i;
	int			tempfd_0;

	int			child_exit_code;
	char		*infile;
	int			num_of_pipes;
	char		*outfile;
	int			has_fd_failed;


	if (g_signal == 130)
		return (130);

	// savestdio[0] = dup(STDIN_FILENO);
	// savestdio[1] = dup(STDOUT_FILENO);
	//PREP
	i = 0;
	while (_pipe[i])
		i ++;
	num_of_pipes = i;
	// fd[0] = -1;
	// fd[1] = -2;
	tempfd_0 = -1;
	//MAIN LOOP
	i = 0;

	while (i < num_of_pipes)
	{
		if (g_signal == 130)
			break;

		infile = NULL;
		outfile = NULL;

		has_fd_failed = 0;
		//PIPING
		if (i != num_of_pipes - 1 && _pipe[1])
			pipe(pipefd);

		if (!has_fd_failed && _pipe[i]->tokens && _pipe[i]->tokens[0] != BUILTIN)
		{
			pid[i] = fork();
			if (pid[i] == 0)
			{
				close(pipefd[0]);
				if (i != 0)
				{
					ft_putendl_fd("duped stdin", 2);
					dup2(tempfd_0, STDIN_FILENO);
					close(tempfd_0);
				}
				if (i != num_of_pipes - 1 && _pipe[1])
					dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);

				if ((_pipe)[i]->cmd_with_path != NULL)
				{
					execve((_pipe)[i]->cmd_with_path, (_pipe)[i]->noio_args, utils->envp);
					child_exit_code = handle_execve_errors((_pipe)[i]->cmd_with_path);
				}
				else if ((_pipe)[i]->noio_args[0])
					child_exit_code = handle_execve_errors((_pipe)[i]->noio_args[0]);
				free_pipes_utils_and_exit(_pipe, utils, child_exit_code);
			}
			else
			{
				if (i != 0)
					close(tempfd_0);
				if (_pipe[1] && i != num_of_pipes - 1)
					tempfd_0 = dup(pipefd[0]);
				close(pipefd[0]);
				close(pipefd[1]);
			}

		}


		else if (!has_fd_failed && _pipe[1])
		{
			pid[i] = fork();
			if (pid[i] == 0)
			{
				utils->err_code = exec_builtin(_pipe, utils, i);
				free_pipes_utils_and_exit(_pipe, utils, utils->err_code);
			}
		}
		else if (!has_fd_failed)
			utils->err_code = exec_builtin(_pipe, utils, i);
		i ++;
	}
	close (pipefd[0]);
	close (pipefd[1]);
	close(tempfd_0);

	i = 0;
	child_exit_code = 0;
	while (_pipe[i])
		i ++;
	num_of_pipes = i;
	i = 0;
	if ((_pipe[i])->tokens && (_pipe[i])->tokens[0] == BUILTIN && !_pipe[1])
			i ++;
	while (i < num_of_pipes)
	{
		// if ((*pid)[i] == -2)
		// 	ft_putendl_fd("error in pid", 2);
		if (i == num_of_pipes - 1)
		{
			//ft_putendl_fd("waiting for child", 2);
			waitpid(pid[i], &child_exit_code, 0);
		}
		else
			waitpid(pid[i], NULL, 0);
		i ++;
	}
	printf("123\n");
	//dup2(savestdio[0], STDIN_FILENO);
	//dup2(savestdio[1], STDOUT_FILENO);
	//ft_putendl_fd("Restoring the STDIO", 2);
	//close(savestdio[0]);
	//close(savestdio[1]);
	//printf("The cild exit code is %d\n", child_exit_code);
	if (g_signal == 130)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (130);
	}
	if (WIFEXITED(child_exit_code) && !has_fd_failed && \
	!(_pipe[0]->tokens[0] == BUILTIN && !_pipe[1]))
	{
		return (WEXITSTATUS(child_exit_code));
	}
	else
		return (utils->err_code);


	//return (waitpid_and_close_exec(_pipe, &pid, savestdio, utils, has_fd_failed));
}

int	waitpid_and_close_exec(t_pipe **_pipe, pid_t (*pid)[256], int savestdio[2], t_utils *utils, int has_fd_failed)
{
	int		i;
	int		child_exit_code;
	int		num_of_pipes;

	i = 0;
	child_exit_code = 0;
	while (_pipe[i])
		i ++;
	num_of_pipes = i;
	i = 0;
	if ((_pipe[i])->tokens && (_pipe[i])->tokens[0] == BUILTIN && !_pipe[1])
			i ++;
	while (i < num_of_pipes)
	{
		// if ((*pid)[i] == -2)
		// 	ft_putendl_fd("error in pid", 2);
		if (i == num_of_pipes - 1)
		{
			//ft_putendl_fd("waiting for child", 2);
			waitpid((*pid)[i], &child_exit_code, 0);
		}
		else if ((*pid)[i] != -2)
			waitpid((*pid)[i], NULL, 0);
		i ++;
	}
	dup2(savestdio[0], STDIN_FILENO);
	dup2(savestdio[1], STDOUT_FILENO);
	//ft_putendl_fd("Restoring the STDIO", 2);
	close(savestdio[0]);
	close(savestdio[1]);
	//printf("The cild exit code is %d\n", child_exit_code);
	if (g_signal == 130)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (130);
	}
	if (WIFEXITED(child_exit_code) && !has_fd_failed && \
	!(_pipe[0]->tokens[0] == BUILTIN && !_pipe[1]))
	{
		//printf("wexitd\n");
		return (WEXITSTATUS(child_exit_code));
	}
	else
		return (utils->err_code);
}


/*
When the name of a builtin command is used as the first word of a simple command (see Simple Commands),
the shell executes the command directly, without invoking another program.
All built­in functions except printenv are executed by the parent process. The reason for this is
that we want setenv, cd etc to modify the state of the parent. If they are executed by the child,
the changes will go away when the child exits. For this built it functions, call the function inside
execute instead of forking a new process
All builtins return an exit status of 2 to indicate incorrect usage, generally invalid options or missing arguments.
*/
int		exec_builtin(t_pipe **_pipe, t_utils *utils, int i)
{

	if (!ft_strncmp((_pipe)[i]->noio_args[0], "echo", -1))
		return (echo_builtin((_pipe)[i]->noio_args, utils));
	else if (!ft_strncmp((_pipe)[i]->noio_args[0], "cd", -1))
		return (cd_builtin(_pipe, utils, i));
	else if (!ft_strncmp((_pipe)[i]->noio_args[0], "pwd", -1))
		return (pwd(utils));
	else if (!ft_strncmp((_pipe)[i]->noio_args[0], "export", -1))
	{
		if (_pipe[0] && !_pipe[1])
			return (export(utils, (_pipe)[i]->noio_args));
		else
			return (0);
	}
	else if (!ft_strncmp((_pipe)[i]->noio_args[0], "unset", -1))
			return (unset(utils, (_pipe)[i]->noio_args));
	else if (!ft_strncmp((_pipe)[i]->noio_args[0], "env", -1))
		return (env(utils));
	else if (!ft_strncmp((_pipe)[i]->noio_args[0], "exit", -1))
		return (exit_builtin(_pipe, utils, i));
	else
	{
		ft_putendl_fd("Error: no builtin for the builtin token", 2);
		return (2);
	}
}

int		handle_execve_errors(char *failed_cmd)
{
	DIR	*dir;
	//ft_putendl_fd(failed_cmd, 2);
	//ACCESS() looks for a command as if it is ./
	if (failed_cmd[0] == 0)
		return (msg_stderr("minishell: permission denied: ", failed_cmd, 126));
	else if (failed_cmd[0] == '.' && failed_cmd[1] == 0)
		return (msg_stderr(".: not enough arguments", NULL, 1));
	else if (access(failed_cmd, F_OK) == -1 && ft_strchr(failed_cmd, '/'))
	{
		return (msg_stderr("minishell: no such file or directory: ", failed_cmd, 127));
	}
	else if (access(failed_cmd, F_OK) == -1)
		return (msg_stderr("minishell: command not found: ", failed_cmd, 127));
	else if (access(failed_cmd, X_OK) == -1 || access(failed_cmd, R_OK) == -1 || \
	ft_strncmp(failed_cmd, "./", 3) == 0)
		return (msg_stderr("minishell: permission denied: ", failed_cmd, 126));
	// else if (!closedir(opendir(failed_cmd)))
	dir = opendir(failed_cmd);
	if (dir)
	{
		closedir(dir);
		if (ft_strnstr(failed_cmd, "../", -1))
			return (msg_stderr("minishell: is a directory: ", ft_strnstr(failed_cmd, "../", -1), 126));
		else if (ft_strnstr(failed_cmd, "./", -1))
			return (msg_stderr("minishell: is a directory: ", ft_strnstr(failed_cmd, "./", -1), 126));
		else
			return (msg_stderr("minishell: is a directory: ", failed_cmd, 126));
	}
	else if (ft_strrchr(failed_cmd, '/') && (ft_strrchr(failed_cmd, '/'))[1] == '\0')
			return (msg_stderr("minishell: Not a directory: ", failed_cmd, 127));
	else
		return (msg_stderr("minishell: command not found: ", failed_cmd, 127));
	return (127);
}

int	msg_stderr(char *message, char *cmd, int err_code)
{
	if (message)
		ft_putstr_fd(message, 2);
	if (cmd)
		ft_putstr_fd(cmd, 2);
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
