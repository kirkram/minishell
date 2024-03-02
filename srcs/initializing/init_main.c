/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:55:30 by klukiano          #+#    #+#             */
/*   Updated: 2024/03/02 16:26:05 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../lib/get_next_line/get_next_line.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

// -L/usr/local/lib -I/usr/local/include -lreadline

char	*rl_gets(char *line_read, int hist_file);
int		rl_loop(int ac, char **av);
int		open_history_file(char *line_read, int hist_fd);
int		one_line_mode(char *line_read, int hist_fd, char **av);
int		interactive_mode_loop(char *line_read, int hist_fd);
int		dash_c_mode(char *line_read, int hist_fd, char **av);


int	main(int ac, char **av)
{
	(void)ac;
	int		ret;

	ret = 0;
	ret = rl_loop(ac, av);

	return (ret);
}

int	rl_loop(int ac, char **av)
{
	static char	*line_read;
	int			hist_fd;

	line_read = NULL;
	//on a specific signal send command to delete the file with unlink()
	//so that it be wiped like we closed the terminal
	hist_fd = open_history_file(line_read, hist_fd);
	if (hist_fd < 0)
		return (1);
	if (ac == 1)
		interactive_mode_loop(line_read, hist_fd);
	else if (ft_strncmp(av[1], "-c", 3) == 0)
		dash_c_mode(line_read, hist_fd, av);
	else
		one_line_mode(line_read, hist_fd, av);
	// interactive_mode;
	//
	// expand_local_script_mode
	free(line_read);
	return (0);
}

int	interactive_mode_loop(char *line_read, int hist_fd)
{

	while (1)
	{
		line_read = rl_gets(line_read, hist_fd);
		//lexer();
	}
	return (0);
}

int	one_line_mode(char *line_read, int hist_fd, char **av)
{
	return (0);
}

int	dash_c_mode(char *line_read, int hist_fd, char **av)
{
	//int	i;

	//dont add history to this mode
	//ignore Commands after the av[2] but not the redirections;
	//but! if there is a pipe everything after that will work!!
	//command should first create the file for output and only then exec commands
	//it will stop executing bash after ';'

	//lexer();
	return (0);
}

int	open_history_file(char *line_read, int hist_fd)
{
	int		err_check;

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
