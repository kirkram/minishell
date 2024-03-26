/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:55:30 by klukiano          #+#    #+#             */
/*   Updated: 2024/03/26 13:38:36 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../lib/get_next_line/get_next_line.h"

int	main(int ac, char **av, char **sys_envp)
{
	(void)ac;
	int		ret;

	signal_handler();
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
	return (interactive_mode_loop(hist_fd, sys_envp));
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
		utils->syntax_err = false;
		line_read = rl_gets(line_read, hist_fd, utils->err_code);
		if (line_read && parsing(&line_read, &_pipe, utils) != 1)
		{
			i = 0;
			while (_pipe[i])
			{
				(_pipe)[i]->cmd_with_path = assign_scmd_path((_pipe)[i]->noio_args[0], utils->envp);
				i ++;
			}
			//should we assign (duplicating) err code to the utils in the execute
			//throughout the execution so that on a sigstop signal
			//it would return a proper code?
			utils->err_code = execute(utils, _pipe);
			i = 0;
			while (_pipe[i])
			{
				ft_arrfree(_pipe[i]->args);
				ft_arrfree(_pipe[i]->noio_args);
				free(_pipe[i]->cmd_with_path);
				free(_pipe[i]->tokens);
				free(_pipe[i]);
				i ++;
			}
			free (_pipe);
		}
	}
	free (utils);
	free (line_read);
	close(hist_fd);
	return (0);
}

char *exp_init(char *str1, char *str2)

{
	int		i;
	int		x;
	bool	equal;
	char	*temp;

	i = 0;
	x = 0;
	equal = false;
	temp = NULL;
	temp = malloc(sizeof(char) * (ft_strlen(str1) + ft_strlen(str2) + 3));
	if (!temp)
		return (NULL);
	while(str1[i])
	{
		temp[i] = str1[i];
		i++;
	}
	while (str2[x])
	{
		if (str2[x] == '=' && equal == false)
		{
			equal = true;
			temp[i] = str2[x];
			temp[i +1] = '\"';
			i += 2;
			x++;
		}
		else
		{
			temp[i] = str2[x];
			x++;
			i++;
		}
	}
	if (equal == true)
		temp[i++] = '\"';
	temp[i] = '\0';
	return (temp);
}

void	intialize_utils(char **sys_envp, t_utils **utils)
{
	int	i;

	*(utils) = malloc(sizeof(t_utils));
	i = 0;
	while (sys_envp[i])
		i ++;
	(*utils)->envp = malloc((i + 1) * sizeof(char *));
	(*utils)->export = malloc((i + 1) * sizeof(char *));
	i = 0;
	while (sys_envp[i])
	{
		(*utils)->envp[i] = ft_strdup(sys_envp[i]);
		(*utils)->export[i] = exp_init("declare -x ", sys_envp[i]);
		//(*utils)->export[i] = ft_strjoin("declare -x ", ft_strdup(sys_envp[i]));
		i ++;
	}
	(*utils)->envp[i] = NULL;
	(*utils)->export[i] = NULL;
	(*utils)->err_code = 0;
	(*utils)->syntax_err = false;
	sort_export(*utils);
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

char *rl_gets(char *line_read, int hist_file, int err_code)
{
	if (line_read)
	{
		free (line_read);
		line_read = NULL;
	}
	line_read = readline("minishell-0.5$ ");
	if (!line_read)
		exit (err_code);
	if (line_read && *line_read)
	{
		add_history(line_read);
		ft_putendl_fd(line_read, hist_file);
	}
	return (line_read);
}
