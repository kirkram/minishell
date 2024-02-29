/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:59:27 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/29 13:42:10 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//test in the file change

#include "../../include/minishell.h"
#include "../../include/kirtemp/ppx_split.h"

int		execute(char **av, char **envp, t_pline *pline);
int		handle_execve_errors(char *failed_cmd);
int		msg_stderr(char *message, char *cmd, int err_code);
char	*find_scmd_path(char *scmd, char **envp);
char	**find_path(char **envp);
char	*jointhree(char const *s1, char const *s2, char const *s3);
int		user_cmd_path(char **args, char *arg_cmd, char **paths);
void	delete_pwd_path(char **paths);
int		free_and_1(char **paths, int **end);


int	main(int ac, char **av, char **envp)
{
	t_pline	pline;
	int			i;
	int			err_code;

	ft_memset(&pline, 0, sizeof(pline));
	pline.infile = av[1];
	pline.outfile = av[ac - 1];
	i = 0;
	pline.cmds = malloc(((ac - 2) + 1) * sizeof(t_scmd *));
	while (i < (ac - 3))
	{
		if (pline.cmds == NULL)
			return (1);
		pline.cmds[i] = malloc(sizeof(t_scmd));
		pline.cmds[i]->args = ppx_split(av[i + 2], ' ');
		//printf("added args for [%d], the arg[0] is %s\n", i, pline.cmds[i]->args[0]);
		pline.cmds[i]->cmd_with_path = find_scmd_path(pline.cmds[i]->args[0], envp);
		//printf("The path for i=[%d] is %s\n", i, pline.cmds[i]->path);
		i ++;
	}
	pline.cmds[i] = NULL;
	pline.num_of_cmds = i;
	//i = 0;
	//t_scmd		**scmds = pline.cmds;
	//printf("The num of cmds is %d\n", pline.num_of_cmds);
	// while (scmds[i])
	// {
	// 	ft_putendl_fd("What the hell?", 2);
	// 	printf("the arg 0 for the [%d] is %s\n", i, scmds[i]->args[0]);
	// 	printf("The path for [%d] is %s\n", i, scmds[i]->path);
	// 	i ++;
	// }
	err_code = execute(av, envp, &pline);
	i = 0;
	while (i < pline.num_of_cmds)
	{
		free(pline.cmds[i]->cmd_with_path);
		free_and_1(pline.cmds[i]->args, NULL);
		i ++;
	}
	i = 0;
	while (pline.cmds[i])
	{
		free (pline.cmds[i]);
		i ++;
	}
	free (pline.cmds);

	//freeing the heap
	return (err_code);
}

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

int	execute(char **av, char **envp, t_pline *pline)
{
	int			fd[2];
	int			savestdio[2];
	pid_t		pid[42];
	int			pipefd[2];
	int			i;

	//these vars are part of the struct pline
	int			err_code;
	char		*infile;
	char		*outfile;
	int			num_of_cmds;
	t_scmd		**scmds;

	(void)		envp;
	(void)		av;

	savestdio[0] = dup(STDIN_FILENO);
	savestdio[1] = dup(STDOUT_FILENO);
	scmds = pline->cmds;

	infile = pline->infile;
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

	err_code = 0;
	num_of_cmds = pline->num_of_cmds;
	i = 0;
	while (i < num_of_cmds)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (i == num_of_cmds - 1)
		{
			if (pline->outfile)
			{
				outfile = pline->outfile;
				fd[1] = open(outfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
				if (fd[1] < 0)
				{
					{
						ft_putstr_fd("minishell: permission denied: ", 2);
						ft_putendl_fd(outfile, 2);
						return (127);
					}
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
		pid[i] = fork();
		if (pid[i] == 0)
		{
			if (scmds[i]->cmd_with_path != NULL)
			{
				execve(scmds[i]->cmd_with_path, scmds[i]->args, NULL);
				err_code = handle_execve_errors(scmds[i]->cmd_with_path);
			}
			else
				err_code = handle_execve_errors(scmds[i]->args[0]);
			exit (err_code);
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
		if (i == num_of_cmds - 1)
			waitpid(pid[i], &err_code, 0);
		else
			waitpid(pid[i], NULL, 0);
		i ++;
	}
	if (WIFEXITED(err_code))
		return (WEXITSTATUS(err_code));
	else
		return (127);
}

/*
All built­in functions except printenv are executed by the parent process. The reason for this is
that we want setenv, cd etc to modify the state of the parent. If they are executed by the child,
the changes will go away when the child exits. For this built it functions, call the function inside
execute instead of forking a new process
*/

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
