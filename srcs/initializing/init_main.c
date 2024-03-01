/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:55:30 by klukiano          #+#    #+#             */
/*   Updated: 2024/03/01 19:00:06 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../lib/get_next_line/get_next_line.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

// -L/usr/local/lib -I/usr/local/include -lreadline

char *rl_gets(char *line_read, int hist_file);

int	main(int ac, char **av)
{
	(void)ac;
	(void)av;
	static char	*line_read;
	//char	*history;
	int		hist_file;
	int		err_check;

	line_read = NULL;
	//on a specific signal send command to delete the file with unlink()
	if (access("/tmp/.mshell_hist", F_OK) == -1)
		hist_file = open("/tmp/.mshell_hist", O_CREAT | O_RDWR, 0644);
	else
	{
		hist_file = open("/tmp/.mshell_hist", O_RDWR, 0644);
		while (1)
		{
			err_check = get_next_line(&line_read, hist_file);
			if (!line_read && err_check == 0)
				break;
			else if (!line_read && err_check == 1)
				break; //error
			add_history(line_read);
			free (line_read);
			line_read = NULL;
		}
	}
	// regular_mode;
	// dash_c_mode;
	// local_script_mode;
	while (1)
	{
		line_read = rl_gets(line_read, hist_file);
	}
	return (1);
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
