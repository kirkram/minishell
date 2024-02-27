/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:59:27 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/27 16:29:26 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//test in the file change

#include "../../include/minishell.h"

int	main(int ac, char **av, char **envp)
{

	if (ac == 5)
		execute(av, envp);
	else
		ft_putendl_fd("Needs 4 arguments", 2);

	return (0);
}

int	execute(char **av, char **envp, t_bigcmd *big_cmd)
{
	int			fd[2];
	int			savestdio[2];
	pid_t		pid[42];
	int			pipefd[2];
	int			i;

	//these vars are part of the struct big_cmd
	int			err_code;
	char const	*infile;
	char const	*outfile;
	int			num_of_cmds;
	t_scmd		**scmds;

	savestdio[0] = dup(STDIN_FILENO);
	savestdio[1] = dup(STDOUT_FILENO);
	scmds = big_cmd->cmds;
	infile = big_cmd->infile;
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
	num_of_cmds = big_cmd->num_of_cmds;
	i = 0;
	while (i < num_of_cmds)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (i == num_of_cmds - 1)
		{
			if (big_cmd->outfile)
			{
				outfile = av[4];
				fd[1] = open(outfile, O_WRONLY, O_TRUNC);
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
			pipe(&pipefd);
			fd[0] = pipefd[0];
			fd[1] = pipefd[1];
		}
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		pid[i] = fork();
		if (pid[i] == 0)
		{
			if (scmds[i]->path != NULL)
				execve(scmds[i]->path, scmds[i]->args, NULL);
			handle_execve_errors(scmds[i]->path);
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
	return (err_code);
}

/*
All built­in functions except printenv are executed by the parent process. The reason for this is
that we want setenv, cd etc to modify the state of the parent. If they are executed by the child,
the changes will go away when the child exits. For this built it functions, call the function inside
execute instead of forking a new process
*/

void	handle_execve_errors(char *failed_cmd)
{
	if (failed_cmd[0] == 0)
		return (msg_stderr("pipex: permission denied: ", failed_cmd, 126));
	else if (failed_cmd[0] == '.' && failed_cmd[1] == 0)
		return (msg_stderr(".: not enough arguments", NULL, 1));
	else if (access(failed_cmd, F_OK) == -1 && ft_strchr(failed_cmd, '/'))
		return (msg_stderr("pipex: no such file or directory ", failed_cmd, 127));
	else if (access(failed_cmd, F_OK) == -1)
		return (msg_stderr("pipex: command not found: ", failed_cmd, 127));
	else if (access(failed_cmd, X_OK) == -1 || access(failed_cmd, R_OK) == -1 || \
	ft_strncmp(failed_cmd, "./", 3) == 0)
		return (msg_stderr("pipex: permission denied: ", failed_cmd, 126));
	else
		return (msg_stderr("pipex: permission denied: ", failed_cmd, 127));
	return (127);
}
