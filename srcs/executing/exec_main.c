/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:59:27 by klukiano          #+#    #+#             */
/*   Updated: 2024/03/11 15:28:15 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//test in the file change

#include "../../include/minishell.h"

char	*find_scmd_path(char *scmd, char **envp)
{
	char	**env_paths;
	char	*cmd_path;
	int		i;

	env_paths = find_path(envp);
	if (!env_paths)
		return (NULL);
	// if (!env_paths && !does_scmd_contain_path(scmd))
	// 	return (NULL);
	//	if scmd contains local or abosolute path in it
	//	then  make the cmd_path an empry string;
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
	pid_t		pid[42];
	int			pipefd[2];
	int			i;
	int			j;

	int			err_code;
	char		*infile;
	int			num_of_cmds;
	char		*outfile;
	int			*tokens;
	char		**all_args;
	int			is_append_out;

	savestdio[0] = dup(STDIN_FILENO);
	savestdio[1] = dup(STDOUT_FILENO);
	outfile = NULL;
	infile = NULL;
	if (infile)
	{
		fd[0] = open (infile, O_RDONLY);
		if (fd[0] < 0)
		{
			ft_putstr_fd("minishell: no such file or directory: ", 2);
			ft_putendl_fd(infile, 2);
		}
	}
	else
		fd[0] = dup(savestdio[0]);

	i = 0;
	while (_pipe[i])
		i ++;
	num_of_cmds = i;

	ft_memset(pid, -1, sizeof(pid));
	// i = -1;
	// while (++i != 41)
	// 	printf("The pid is %d\n", pid[i]);
	// while (1);
	err_code = 0;
	i = 0;
	is_append_out = 0;
	while (i < num_of_cmds)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);

		// I need outfile otherwise I have to look for it every time there is a new _pipe
		tokens = _pipe[i]->tokens;
		all_args = _pipe[i]->args;
		j = -1;
		//must be really careful that the amount of tokens equals amount of args
		while (all_args[++j] && tokens[j] != 0)
		{
			if (tokens[j] == OUT || tokens[j] == OUT_AP)
			{
				if (tokens[j] == OUT_AP)
					is_append_out = 1;
				outfile = all_args[j];
				//!!!!!!!!!!!!!!
				//args[j] = NULL;
				// ft_putstr_fd("The outfile is %s\n", 2);
				// ft_putendl_fd(outfile, 2);
			}
			if (tokens[j] == 0)
				ft_putendl_fd("UNEQUAL TOKENS AND ARGS COUNT", 2);
		}
		///////////////////
		if (i == num_of_cmds - 1)
		{
			if (outfile)
			{
				if (!is_append_out)
					fd[1] = open(outfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
				else
					fd[1] = open(outfile, O_WRONLY | O_APPEND);
				if (fd[1] < 0)
				{
					ft_putstr_fd("minishell: permission denied: ", 2);
					ft_putendl_fd(outfile, 2);
					return (127);
				}
			}
			else
				fd[1] = dup(savestdio[1]);
		}
		else
		{
			pipe(pipefd);
			fd[0] = pipefd[0];
			fd[1] = pipefd[1];
		}
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		if (tokens && tokens[0] != BUILTIN)
		{
			pid[i] = fork();
			if (pid[i] == 0)
			{
				if ((_pipe)[i]->cmd_with_path != NULL)
				{
					execve((_pipe)[i]->cmd_with_path, (_pipe)[i]->noio_args, NULL);
					err_code = handle_execve_errors((_pipe)[i]->cmd_with_path);
				}
				else
					err_code = handle_execve_errors((_pipe)[i]->noio_args[0]);
				if (i != num_of_cmds - 1)
					err_code = 127;
				exit (err_code);
			}
		}
		else
		{
			if	(exec_builtin(_pipe, utils, i))
				//handle errors
				return (2);
		}
		i ++;
	}
	dup2(savestdio[0], STDIN_FILENO);
	dup2(savestdio[1], STDOUT_FILENO);
	close(savestdio[0]);
	close(savestdio[1]);

	i = 0;
	while (i < num_of_cmds)
	{
		if ((_pipe[i])->tokens && (_pipe[i])->tokens[0] == BUILTIN)
			i ++;
		if (i == num_of_cmds - 1 && pid[i] != -1)
			waitpid(pid[i], &err_code, 0);
		else if (pid[i] != -1)
			waitpid(pid[i], NULL, 0);
		i ++;
	}
	if (WIFEXITED(err_code))
	{
		//ft_putendl_fd("WIFEXITED", 2);
		return (WEXITSTATUS(err_code));
	}
	else
	{
		//ft_putendl_fd("NOT WIFEXITED", 2);
		return (127);
	}

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
		return (echo_builtin((_pipe)[i]->noio_args));
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


char	**find_path(char **envp)
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
