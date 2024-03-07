/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:55:30 by klukiano          #+#    #+#             */
/*   Updated: 2024/03/07 15:02:28 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../lib/get_next_line/get_next_line.h"

int	main(int ac, char **av, char **sys_envp)
{
	(void)ac;
	int		ret;


	ret = 0;
	ret = rl_loop(ac, av, sys_envp);
	return (ret);
}

int	rl_loop(int ac, char **av, char **sys_envp)
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
	 //what are the other possible conditions to use interactive mode with more than 1 arg?
	return (interactive_mode_loop(hist_fd, sys_envp));
		//expand_scripts_mode(av);
	// interactive_mode;
	//
	// expand_local_script_mode
}

int	interactive_mode_loop(int hist_fd, char **sys_envp)
{
	char	*line_read;
	t_pipe	**_pipe;
	int		i;
	t_utils	*utils;

	intialize_utils(sys_envp, &utils);
	line_read = NULL;
	while (1)
	{
		line_read = rl_gets(line_read, hist_fd);

		if (lexer(line_read, &_pipe, &utils) != 1)
		{
			i = 0;
			while (_pipe[i])
			{
				(_pipe)[i]->cmd_with_path = find_scmd_path((_pipe)[i]->args[0], utils->envp);
				i ++;
			}
			execute(utils, _pipe);
		}
	}
	free(line_read);
	return (0);
}

void	intialize_utils(char **sys_envp, t_utils **utils)
{
	int i;

	*utils = malloc(sizeof(t_utils));
	i = 0;
	while(sys_envp[i])
		i ++;
	(*utils)->envp = malloc((i + 1) * sizeof(char *));
	i = 0;
	while(sys_envp[i])
	{
		(*utils)->envp[i] = ft_strdup(sys_envp[i]);
		i ++;
	}
	(*utils)->envp[i] = NULL;
	(*utils)->err_code = 0;
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

