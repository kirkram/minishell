/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:55:30 by klukiano          #+#    #+#             */
/*   Updated: 2024/03/06 23:14:07 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../lib/get_next_line/get_next_line.h"

//#include <readline/readline.h>
//#include <readline/history.h>
#include <signal.h>

// -L/usr/local/lib -I/usr/local/include -lreadline

char	*rl_gets(char *line_read, int hist_file);
int		rl_loop(int ac, char **av, char **envp);
int		open_history_file(int hist_fd);
int		one_line_mode(char **av);
int		interactive_mode_loop(int hist_fd, char **envp);
int		dash_c_mode(char **av);


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
	// if (ac == 1) //what are the other possible conditions to use interactive mode with more than 1 arg?
		interactive_mode_loop(hist_fd, envp);
	// else if (ft_strncmp(av[1], "-c", 3) == 0)
	// 	dash_c_mode(av);
	// else
	// 	one_line_mode(av);
	// interactive_mode;
	//
	// expand_local_script_mode
	return (0);
}
int	dash_c_mode(char **av)
{
	char	*line_read;
	int		i;
	size_t	len;

	i = 0;
	len = 0;
	while (av[++i])
		len += ft_strlen(av[i]);
	line_read = ft_calloc(1, len + 1);
	printf ("The len is %zu and string is %s\n", len, line_read);

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

int	one_line_mode(char **av)
{
	(void) av;
	char	*line_read;

	printf("One line mode\n");
	line_read = NULL;
	//dont add history to this mode

	return (0);
}


int	open_history_file(int hist_fd)
{
	int		err_check;
	char	*line_read;

	if (access("/tmp/.mshell_hist", F_OK) == -1)
		hist_fd = open("/tmp/.mshell_hist", O_CREAT | O_RDWR, 0644);
	else
	{
		hist_fd = open("/tmp/.mshell_hist", O_RDWR, 0644);
		while (1)
		{
			err_check = get_next_line(&line_read, hist_fd);
			if (!line_read && err_check == 0)
				break;
			else if (!line_read && err_check == 1)
			{
				ft_putendl_fd("HANDLE ERROR IN GNL", 2);
				break; //error
			}
			add_history(line_read);
			free (line_read);
			line_read = NULL;
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
