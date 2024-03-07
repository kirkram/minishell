/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:55:30 by klukiano          #+#    #+#             */
/*   Updated: 2024/03/07 10:18:55 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../lib/get_next_line/get_next_line.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

// -L/usr/local/lib -I/usr/local/include -lreadline

char	*rl_gets(char *line_read, int hist_file);
int		rl_loop(int ac, char **av, char **envp);
int		open_history_file(int hist_fd);
int		scripts_mode(char **av, char **envp);
int		interactive_mode_loop(int hist_fd, char **envp);
int		dash_c_mode(char **av, char **envp);


int	main(int ac, char **av, char **envp)
{
	(void)ac;
	int		ret;

	ret = 0;
	ret = rl_loop(ac, av, envp);
	return (ret);
}

int	rl_loop(int ac, char **av, char **envp)
{
	int			hist_fd;
	(void)ac;
	(void)av;
	//on a specific signal send command to delete the file with unlink()
	//so that it be wiped like we closed the terminal
	hist_fd = -1;
	hist_fd = open_history_file(hist_fd);
	if (hist_fd < 0)
		return (1);
	if (ac == 1) //what are the other possible conditions to use interactive mode with more than 1 arg?
		interactive_mode_loop(hist_fd, envp);
	else if (ft_strncmp(av[1], "-c", 3) == 0)
		dash_c_mode(av, envp);
	else
		scripts_mode(av, envp);
		//expand_scripts_mode(av);
	// interactive_mode;
	//
	// expand_local_script_mode
	return (0);
}

int	dash_c_mode(char **av, char **envp)
{
	char	*line_read;
	int		i;
	t_pipe	**_pipe;
	int		err_code;

	line_read = NULL;
	line_read = ft_calloc(1, ft_strlen(av[2]) + 1);
	if (!line_read)
	{
		//HANDLE ERRORS;
		return (1);
	}
	ft_strlcat(line_read, av[2], -1);
	_pipe = lexer(line_read, envp);
	i = 0;
	while (_pipe[i])
	{
		(_pipe)[i]->cmd_with_path = find_scmd_path((_pipe)[i]->args[0], envp);
		i ++;
	}
	err_code = execute(envp, _pipe);
	//the only difference is exclusion of everything besides redirectons there after " "?
	//dont add history to this mode
	//ignore Commands after the av[2] but not the redirections;
	//but! if there is a pipe everything after that will work!!
	//command should first create the file for output and only then exec commands
	//it will stop executing bash after ';'
	//lexer();
	return (0);

}
int	scripts_mode(char **av, char **envp)
{
	char	*line_read;

	int		i;
	t_pipe	**_pipe;
	int		err_code;
	size_t		len;

	//from bash(1)
	//ARGUMENTS If arguments remain after option processing,
	//and neither the -c nor the -s option has been supplied,
	//the first argument is assumed to be the name of a file containing shell commands.

	ft_putendl_fd("IMPLEMENT OPENING OF THE SCRIPTS, RETURNING...", 2);
	return (1);
	i = 1;
	len = 0;
	while (av[i])
	{
		if (!ft_strncmp(av[i], ">", -1) || !ft_strncmp(av[i], "|", -1) ||
		 !ft_strncmp(av[i], ">>", -1))
			break ;
		len += ft_strlen(av[i]) + 1;
		i ++;
	}
	line_read = NULL;
	line_read = ft_calloc(1, len + 1);
	if (!line_read)
	{
		//HANDLE ERRORS;
		return (1);
	}
	i = 1;
	while (av[i])
	{
		ft_strlcat(line_read, av[i], -1);
		if (av[i + 1])
			ft_strlcat(line_read, " ", -1);
		i ++;
	}
	_pipe = lexer(line_read, envp);
	free (line_read);
	i = 0;
	while (_pipe[i])
	{
		(_pipe)[i]->cmd_with_path = find_scmd_path((_pipe)[i]->args[0], envp);
		i ++;
	}
	err_code = execute(envp, _pipe);
	//the only difference is exclusion of everything besides redirectons there after " "?
	//dont add history to this mode
	//ignore Commands after the av[2] but not the redirections;
	//but! if there is a pipe everything after that will work!!
	//command should first create the file for output and only then exec commands
	//it will stop executing bash after ';'
	//lexer();
	return (0);
}

int	interactive_mode_loop(int hist_fd, char **envp)
{
	char	*line_read;
	t_pipe	**_pipe;
	int		i;
	int		err_code;

	line_read = NULL;
	while (1)
	{
		line_read = rl_gets(line_read, hist_fd);
		
		if (lexer(line_read, envp, &_pipe, &err_code) != 1)
		{
			i = 0;
			while (_pipe[i])
			{
				(_pipe)[i]->cmd_with_path = find_scmd_path((_pipe)[i]->args[0], envp);
				i ++;
			}
			err_code = execute(envp, _pipe);
		}
	}
	free(line_read);
	return (0);
}


int	open_history_file(int hist_fd)
{
	int		err_check;
	char	*gnl_line;

	gnl_line = NULL;
	err_check = 0;
	if (access("/tmp/.mshell_hist", F_OK) == -1)
		hist_fd = open("/tmp/.mshell_hist", O_CREAT | O_RDWR, 0644);
	else
	{
		hist_fd = open("/tmp/.mshell_hist", O_RDWR, 0644);
		while (1)
		{
			err_check = get_next_line(&gnl_line, hist_fd);
			if (!gnl_line && err_check == 0)
				break;
			else if (!gnl_line && err_check == 1)
			{
				ft_putendl_fd("HANDLE ERROR IN GNL", 2);
				break; //error HANDLE
			}
			add_history(gnl_line);
			free (gnl_line);
			gnl_line = NULL;
		}
	}
	return (hist_fd);
}

char *rl_gets(char *line_read, int hist_file)
{
	if (line_read)
	{
		free (line_read);
		line_read = NULL;
	}
	line_read = readline("minishell-0.1$ ");
	if (line_read && *line_read)
	{
		add_history(line_read);
		ft_putendl_fd(line_read, hist_file);
	}
	return (line_read);
}

