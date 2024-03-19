/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:59:27 by klukiano          #+#    #+#             */
/*   Updated: 2024/03/19 17:47:39 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//test in the file change

#include "../../include/minishell.h"

char	*assign_scmd_path(char *scmd, char **envp)
{
	char	**env_paths;
	char	*cmd_path;
	int		i;

	env_paths = find_path_and_pwd(envp);
	if (!env_paths)
		return (NULL);
	i = 0;
	while (env_paths[i])
	{
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
	int			is_append_out;
	int			is_fd_failed;

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


	//CREATE OUTFILES
	i = 0;
	while (i < num_of_pipes && i < 256)
	{
		j = 0;
		tokens = _pipe[i]->tokens;
		while (_pipe[i]->args[j] && tokens[j] != 0)
		{
			if (tokens[j] == OUT)
				fd[0] = open(_pipe[i]->args[j], O_CREAT | O_RDWR | O_TRUNC, 0644);
			else if (tokens[j] == OUT_AP)
				fd[0] = open(_pipe[i]->args[j], O_CREAT | O_RDWR | O_APPEND, 0644);
			else if (tokens[j] == SKIP_OUT)
				fd[0] = open(_pipe[i]->args[j], O_CREAT | O_RDWR);
			if (tokens[j] == OUT || tokens[j] == OUT_AP || tokens[j] == SKIP_OUT)
			{
				//system couldnt create a file, abort everything
				if (access(_pipe[i]->args[j], F_OK == -1))
					return (127);
				//we dont them opened right now so we close it instantly
				close(fd[0]);
			}
			j ++;
		}
		i ++;
	}

	//MAIN LOOP
	i = 0;
	while (i < num_of_pipes)
	{

		//OPEN INFILE AND TRY TO ACCESS
		is_fd_failed = 0;
		infile = NULL;
		j = 0;
		while(_pipe[i]->args[j])
		{
			if (_pipe[i]->tokens[j] == IN_FD)
			{
				infile = _pipe[i]->args[j];
				fd[0] = open (infile, O_RDONLY);
				if (fd[0] < 0)
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(infile, 2);
					if (access(infile, F_OK) == -1)
						ft_putendl_fd(": No such file or directory", 2);
					else if (access(infile, R_OK) == -1)
						ft_putendl_fd(": Permission denied", 2);
					is_fd_failed = 1;
					//STDIN should not work if the open failed anyway (?)
				}
			}
			else if (_pipe[i]->tokens[j] == SKIP_IN)
			{
				if (access(_pipe[i]->args[j], F_OK) == -1 || access(_pipe[i]->args[j], R_OK) == -1)
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(_pipe[i]->args[j], 2);
					if (access(_pipe[i]->args[j], F_OK) == -1)
						ft_putendl_fd(": No such file or directory", 2);
					else if (access(_pipe[i]->args[j], R_OK) == -1)
						ft_putendl_fd(": Permission denied", 2);
					is_fd_failed = 1;
					//STDIN should not work if the open failed anyway (?)
				}
			}
			j ++;
		}
		if (!infile && i == 0)
			fd[0] = dup(savestdio[0]);


		//LOOK FOR AN OUTFILE
		tokens = _pipe[i]->tokens;
		outfile = NULL;
		j = -1;
		is_append_out = 0;
		while (_pipe[i]->args[++j] && tokens[j] != 0)
		{
			if (tokens[j] == OUT || tokens[j] == OUT_AP)
			{
				if (tokens[j] == OUT_AP)
					is_append_out = 1;
				outfile = _pipe[i]->args[j];
			}
			//NEW REDIR LOGIC
			else if (tokens[j] == SKIP_OUT)
			{
				//No wr/read rights kill the whole outfile even if its not the true outfile
				if(!access(_pipe[i]->args[j], F_OK) && access(_pipe[i]->args[j], W_OK) == -1)
				{
					outfile = _pipe[i]->args[j];
					break ;
				}
				//if it fails to open then dont pass anything to the other outfile
				//we make it go through so the STDOUT wouldnt work
			}
			else if (tokens[j] == 0)
				ft_putendl_fd("UNEQUAL TOKENS AND ARGS COUNT", 2);
		}
		if (outfile)
		{
			if (!is_append_out && fd[1] >= 0)
				fd[1] = open(outfile, O_RDWR | O_TRUNC, 0644);
			else if (is_append_out && fd[1] >= 0)
				fd[1] = open(outfile, O_RDWR | O_APPEND, 0644);
			if (fd[1] < 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(outfile, 2);
				ft_putendl_fd(": Permission denied", 2);
				is_fd_failed = 1;
				//NEW REDIR LOGIC
				//return (127);
			}
		}
		else if (i == num_of_pipes - 1)
			fd[1] = dup(savestdio[1]);

		//PIPING

		// dup2(fd[0], STDIN_FILENO);
		// close(fd[0]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (i != num_of_pipes - 1)
		{
			pipe(pipefd);
			//if (!infile)
				fd[0] = pipefd[0];
			// else
			// 	close(pipefd[0]);
			if (!outfile)
				fd[1] = pipefd[1];
			else
				close(pipefd[1]);
		}
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		if (tokens && tokens[0] != BUILTIN && !is_fd_failed)
		{
			pid[i] = fork();
			if (pid[i] == 0)
			{
				if ((_pipe)[i]->cmd_with_path != NULL)
				{
					execve((_pipe)[i]->cmd_with_path, (_pipe)[i]->noio_args, utils->envp);
					child_exit_code = handle_execve_errors((_pipe)[i]->cmd_with_path);
				}
				else
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
		else if (!is_fd_failed)
		{
			//close(pipefd[0]);
			utils->err_code = exec_builtin(_pipe, utils, i);
		}
		i ++;
	}
	dup2(savestdio[0], STDIN_FILENO);
	dup2(savestdio[1], STDOUT_FILENO);
	close(savestdio[0]);
	close(savestdio[1]);

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
	// while (1);
	//
	if (WIFEXITED(child_exit_code))
		return (WEXITSTATUS(child_exit_code));
	else
		return (child_exit_code);
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
	//is access command slow?
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
	else if (!closedir(opendir(failed_cmd)))
	{
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


char	**find_path_and_pwd(char **envp)
{
	t_paths	vars;

	vars.paths = NULL;
	vars.i = 0;
	vars.path = NULL;
	vars.pwd = NULL;
	while (envp[vars.i])
	{
		if (ft_strncmp(envp[vars.i], "PATH=", 5) == 0)
			vars.path = envp[vars.i] + 5;
		else if (ft_strncmp(envp[vars.i], "PWD=", 4) == 0)
			vars.pwd = envp[vars.i] + 4;
		vars.i ++;
	}
	vars.bigpath = jointhree(vars.path, ":", vars.pwd);
	if (vars.pwd && vars.path)
		vars.paths = ft_split(vars.bigpath, ':');
	else if (vars.path)
		vars.paths = ft_split(vars.path, ':');
	else
		vars.paths = ft_split(vars.pwd, ':');
	free (vars.bigpath);
	return (vars.paths);
}

char	*jointhree(char const *s1, char const *s2, char const *s3)
{
	char	*newstr;

	if (s1 && s2 && s3)
	{
		newstr = malloc(((ft_strlen(s1) + ft_strlen(s2) \
		+ ft_strlen(s3)) + 1) * sizeof(char));
		if (newstr == NULL)
			return (NULL);
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
		return (free_and_1(args + 1, NULL));
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
