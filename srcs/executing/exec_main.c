/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:59:27 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/02 15:42:28 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//test in the file change

#include "../../include/minishell.h"

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

int	execute(t_utils *utils, t_pipe **_pipe)
{
	int			fd[2];
	int			savestdio[2];
	pid_t		pid[256]; //zsh limit descriptors = 256
	int			pipefd[2];
	int			i;
	int			j;

	int			child_exit_code;
	char		*infile;
	int			num_of_pipes;
	char		*outfile;
	int			*tokens;
	int			has_fd_failed;


	if (g_signal == 130)
		return (130);

	savestdio[0] = dup(STDIN_FILENO);
	savestdio[1] = dup(STDOUT_FILENO);
	//PREP
	i = 0;
	while (_pipe[i])
		i ++;
	num_of_pipes = i;
	ft_memset(pid, -1, sizeof(pid));
	fd[0] = 0;
	fd[1] = 0;
	//MAIN LOOP
	i = 0;
	while (i < num_of_pipes)
	{
		if (g_signal == 130)
			break;
		//OPEN INFILE, OPEN/CREATE OUTFILE AND TRY TO ACCESS
		infile = NULL;
		outfile = NULL;
		tokens = _pipe[i]->tokens;
		has_fd_failed = 0;
		j = 0;
		while(_pipe[i]->args[j] && tokens[j] != 0)
		{
			if (_pipe[i]->tokens[j] == SKIP_IN || _pipe[i]->tokens[j] == IN_FD || _pipe[i]->tokens[j] == IN_HD)
			{
				if (_pipe[i]->tokens[j] == IN_FD)
				{
					fd[0] = open (_pipe[i]->args[j], O_RDONLY);
					infile = _pipe[i]->args[j];
				}
				else if (_pipe[i]->tokens[j] == IN_HD)
				{
					fd[0] = _pipe[i]->hd_fd[0];
					infile = _pipe[i]->args[j];
				}
				if (_pipe[i]->tokens[j] != IN_HD && \
				(access(_pipe[i]->args[j], F_OK) == -1 || access(_pipe[i]->args[j], R_OK) == -1))
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(_pipe[i]->args[j], 2);
					if (access(_pipe[i]->args[j], F_OK) == -1)
						ft_putendl_fd(": No such file or directory", 2);
					else if (access(_pipe[i]->args[j], R_OK) == -1)
						ft_putendl_fd(": Permission denied", 2);
					has_fd_failed = 1;
					break ;
				}
			}
			else if (tokens[j] == SKIP_OUT || tokens[j] == OUT || tokens[j] == OUT_AP)
			{
				if (tokens[j] == SKIP_OUT)
					fd[1] = open(_pipe[i]->args[j], O_CREAT | O_WRONLY, 0644);
				else if (tokens[j] == OUT)
					fd[1] = open(_pipe[i]->args[j], O_CREAT | O_WRONLY | O_TRUNC, 0644);
				else if (tokens[j] == OUT_AP)
					fd[1] = open(_pipe[i]->args[j], O_CREAT | O_WRONLY | O_APPEND, 0644);
				if (fd[1] < 0)
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(_pipe[i]->args[j], 2);
					ft_putendl_fd(": Permission denied", 2);
					has_fd_failed = 1;
					break ;
				}
				else if (tokens[j] != SKIP_OUT)
					outfile = _pipe[i]->args[j];
			}
			else if (tokens[j] == 0)
				ft_putendl_fd("UNEQUAL TOKENS AND ARGS COUNT", 2);
			j ++;
		}
		if (!infile && i == 0)
			fd[0] = dup(savestdio[0]);
		if (!outfile && i == num_of_pipes - 1)
			fd[1] = dup(savestdio[1]);
		if (has_fd_failed)
			utils->err_code = 1;

		//PIPING
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (i != num_of_pipes - 1)
		{
			pipe(pipefd);
			fd[0] = pipefd[0];
			if (!outfile)
				fd[1] = pipefd[1];
			else
				close(pipefd[1]);
		}
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		if (!has_fd_failed && tokens && tokens[0] != BUILTIN)
		{
			pid[i] = fork();
			if (pid[i] == 0)
			{
				if ((_pipe)[i]->cmd_with_path != NULL)
				{
					//plus_one_to_shlvl_variable(utils, (_pipe)[i]->cmd_with_path);
					execve((_pipe)[i]->cmd_with_path, (_pipe)[i]->noio_args, utils->envp);
					child_exit_code = handle_execve_errors((_pipe)[i]->cmd_with_path);
				}
				else if ((_pipe)[i]->noio_args[0])
					child_exit_code = handle_execve_errors((_pipe)[i]->noio_args[0]);
				if (i != num_of_pipes - 1)
					child_exit_code = 127;
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
				ft_arrfree(utils->envp);
				ft_arrfree(utils->export);
				free(utils);
				free (_pipe);
				exit (child_exit_code);
			}
		}
		else if (!has_fd_failed)
		{
			utils->err_code = exec_builtin(_pipe, utils, i);
		}
		i ++;
	}

	i = 0;
	while (i < num_of_pipes)
	{
		if ((_pipe[i])->tokens && (_pipe[i])->tokens[0] == BUILTIN)
			i ++;
		if (i == num_of_pipes - 1 && pid[i] != -1)
			waitpid(pid[i], &child_exit_code, 0);
		else if (pid[i] != -1)
			waitpid(pid[i], NULL, 0);
		i ++;
	}
	dup2(savestdio[0], STDIN_FILENO);
	dup2(savestdio[1], STDOUT_FILENO);
	close(savestdio[0]);
	close(savestdio[1]);
	if (g_signal == 130)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (130);
	}
	// while (1);
	if (WIFEXITED(child_exit_code))
		return (WEXITSTATUS(child_exit_code));
	else
		return (utils->err_code);
}

//Assumes that the minishell is run from the current folder (PWD)
//Ignores ./ and ../ after PWD and looks for minishell name without anything afterwards instead
// void	plus_one_to_shlvl_variable(t_utils *utils, char *cmd_with_path)
// {
// 	int		i;
// 	int		j;
// 	int		lvl;
// 	char	*newstr;
// 	char	*shlvl;
// 	int		pwd_len;

// 	i = 0;
// 	j = 0;
// 	lvl = -1;
// 	shlvl = NULL;
// 	while (utils->envp[i])
// 	{
// 		//ft_putendl_fd(utils->envp[i], 2);
// 		if (ft_strncmp(utils->envp[i], "PWD=", 4) == 0)
// 		//if we find the match until the pwd then if there is word minishell with no characters after
// 		{
// 			ft_putendl_fd("!looking for shell name", 2);
// 			pwd_len = ft_strlen(utils->envp[i] + 4);
// 			if (ft_strncmp(cmd_with_path, utils->envp[i] + 4, pwd_len) == 0)
// 			{
// 				newstr = ft_strnstr(cmd_with_path + pwd_len, "minishell", -1);
// 				if (!newstr)
// 					newstr = ft_strnstr(cmd_with_path + pwd_len, "bash", -1);
// 				if (!newstr)
// 					newstr = ft_strnstr(cmd_with_path + pwd_len, "zsh", -1);
// 				if (newstr && (newstr + ft_strlen(newstr))[0] == '\0')
// 				{
// 					lvl = 1;
// 					ft_putendl_fd("!!!! success in finding minishell exec in pwd and no extra char", 2);
// 				}
// 				newstr = NULL;
// 			}
// 			// else
// 			// {
// 			// 	ft_putendl_fd(ft_itoa(ft_strlen(utils->envp[i])), 2);
// 			// 	ft_putendl_fd(cmd_with_path, 2);
// 			// 	ft_putendl_fd(utils->envp[i] + 4, 2);
// 			// }
// 		}
// 		else if (ft_strncmp(utils->envp[i], "SHLVL=", 6) == 0)
// 		{
// 			shlvl = utils->envp[i];
// 			// ft_putendl_fd("!!!!!   found shlvl!   ", 2);
// 		}
// 		i ++;
// 	}
// 	if (!shlvl || lvl == -1)
// 	{
// 		// if (!shlvl)
// 		// 	ft_putendl_fd("fail on shlvl find" , 2);
// 		// if (lvl == -1)
// 		// 	ft_putendl_fd("fail on equaling pwd to minishell" , 2);
// 		return ;
// 	}
// 	// ft_putendl_fd("enter plus one" , 2);
// 	i = 6;
// 	while (shlvl[i])
// 	{
// 		if (ft_isdigit(shlvl[i]))
// 		{
// 			lvl = ft_atoi(shlvl + i) + 1;
// 			newstr = ft_itoa(lvl);
// 			if (!newstr)
// 				malloc_error(1);
// 			shlvl = ft_strjoin("SHLVL=", newstr);
// 			if (!shlvl)
// 				malloc_error(1);
// 			change_env_var(&utils, "SHLVL=", shlvl);
// 			//ft_putendl_fd(shlvl, 2);
// 			free(shlvl);
// 			return ;
// 		}
// 		i ++;
// 	}
// }

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
		return (export(utils, (_pipe)[i]->noio_args));
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
	//ACCESS() looks for a command as if it is ./
	if (failed_cmd[0] == 0)
		return (msg_stderr("minishell: permission denied: ", failed_cmd, 126));
	else if (failed_cmd[0] == '.' && failed_cmd[1] == 0)
		return (msg_stderr(".: not enough arguments", NULL, 1));
	else if (access(failed_cmd, F_OK) == -1 && ft_strchr(failed_cmd, '/'))
		return (msg_stderr("minishell: no such file or directory ", failed_cmd, 127));
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
	else
		return (msg_stderr("minishell: permission denied: ", failed_cmd, 127));
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
