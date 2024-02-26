/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:59:27 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/26 18:37:45 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//test in the file change

#include "../../include/minishell.h"

// int	child_first()
// int	child_middle();
// int	child_last();

void	execute(char **av, char **envp);

int	main(int ac, char **av, char **envp)
{

	if (ac == 5)
		execute(av, envp);
	else
		ft_putendl_fd("Needs 4 arguments", 2);

	return (0);
}

void	execute(char **av, char **envp, t_cmd *big_cmd)
{
	/*The strategy for your shell is to have the parent process do all the piping
	and redirection before forking the processes.
	In this way the children will inherit the redirection. The parent
	needs to save input/output and restore it at the end.
	Stderr is the same for all processes */

	int	fd[2];
	///int	fdout[4096];
	int	savestdio[2];
	int	pid[42];
	int	pipefd[2];
	int	i;
	t_cmd *ptr;
	char const *infile;
	char const *outfile;
	char	**cmd;
	int		num_of_cmds;

	savestdio[0] = dup(STDIN_FILENO);
	savestdio[1] = dup(STDOUT_FILENO);

	//if (cmd->infile) meaning if there is redirection
	infile = av[1];
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


	cmd = big_cmd->cmd;
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
				fd[1] = open(outfile, O_WRONLY | O_RDWR | O_TRUNC, 0644);
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
		dup2(fd[1], 1);
		close(fd[1]);



		i ++;
	}




	// if (access(infile, F_OK) == 0 && access(infile, R_OK) == 0)
	// 	dup2(fd[0], STDIN_FILENO);

	return (0);
}


/*


// Command Data Structure
// Describes a simple command and arguments
struct SimpleCommand {
        // Available space for arguments currently preallocated
        int _numberOfAvailableArguments;
        // Number of arguments
        int _numberOfArguments;
        // Array of arguments
        char ** _arguments;
        SimpleCommand();
        void insertArgument( char * argument );
};
// Describes a complete command with the multiple pipes if any
// and input/output redirection if any.
struct Command {
        int _numberOfAvailableSimpleCommands;
        int _numberOfSimpleCommands;
        SimpleCommand ** _simpleCommands;
        char * _outFile;
        char * _inputFile;
        char * _errFile;
        int _background;
        void prompt();
        void print();
        void execute();
        void clear();
        Command();
        void insertSimpleCommand( SimpleCommand * simpleCommand );
        static Command _currentCommand;
        static SimpleCommand *_currentSimpleCommand;
};

*/
