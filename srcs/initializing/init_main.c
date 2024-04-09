/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:55:30 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/09 15:35:28 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../lib/get_next_line/get_next_line.h"

int	g_signal;

int	main(int ac, char **av, char **sys_envp)
{

	(void)ac;
	int		ret;
	g_signal = 0;

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
	//char	*line_read;
	//t_pipe	**_pipe;
	int		i;
	//t_utils	*utils;
	t_ms	ms;

	ms.line = NULL;
	intialize_utils(sys_envp, &(ms.utils));
	//line_read = NULL;
	while (1)
	{
		g_signal = 0;
		//ms.utils->syntax_err = false;
		//line_read = rl_gets(line_read, hist_fd, ms.utils);
		ms.line = rl_gets(ms.line, hist_fd, ms.utils);
		//if (line_read && parsing(&line_read, &ms) != 1)
		if (ms.line && parsing(&ms) != 1)
		{
			i = -1;
			while (ms.pipe[++i])
				ms.pipe[i]->cmd_with_path = assign_scmd_path(ms.pipe[i]->noio_args[0], ms.utils->envp);//&ms);
			ms.utils->err_code = execute(ms.utils, ms.pipe);   //(&ms); //just for compiling
		}
		free_pipes_utils_and_exit(ms.pipe, NULL, -42);
	}
	free_pipes_utils_and_exit(NULL, ms.utils, -42);
	free (ms.line);
	//free (line_read);
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
		malloc_error(1);
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

char	*shell_level(char *str)
{
	int	i;
	int	start;
	int	lvl;
	char *temp;

	temp = NULL;
	lvl = 0;
	start = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			start = i;
			while (str[i])
				i++;
			temp = ft_substr(str, start, i - start);
			if (!temp)
				malloc_error (1);
			lvl = ft_atoi(temp);
			free (temp);
			lvl++;
			return (ft_free_strjoin(ft_substr(str, 0, start), ft_itoa(lvl)));
		}
		i++;
	}
	return (ft_strdup(str));

}

void	intialize_utils(char **sys_envp, t_utils **utils)
{
	// mallloc error needs to bee fixed
	int		i;

	*(utils) = malloc(sizeof(t_utils));
	if (*utils == NULL)
		malloc_error(1);
	i = 0;
	while (sys_envp[i])
		i ++;
	(*utils)->envp = malloc((i + 1) * sizeof(char *));
	if ((*utils)->envp == NULL)
		malloc_error(1);
	(*utils)->export = malloc((i + 1) * sizeof(char *));
	if ((*utils)->export == NULL)
		malloc_error(1);
	i = 0;
	while (sys_envp[i])
	{
		if (ft_strncmp("SHELL=", sys_envp[i], 6) == 0)
		{
			(*utils)->envp[i] = ft_strjoin("SHELL=", "minishell");
			(*utils)->export[i] = exp_init("declare -x ", (*utils)->envp[i]);
		}
		else if (ft_strncmp("SHLVL=", sys_envp[i], 6) == 0)
		{
			(*utils)->envp[i] = shell_level(sys_envp[i]);
			if (!(*utils)->envp[i])
				malloc_error(1);
			(*utils)->export[i] = exp_init("declare -x ", (*utils)->envp[i]);
		}
		else
		{
			(*utils)->envp[i] = ft_strdup(sys_envp[i]);
			(*utils)->export[i] = exp_init("declare -x ", sys_envp[i]);
		}
		i ++;
	}
	(*utils)->envp[i] = NULL;
	(*utils)->export[i] = NULL;
	(*utils)->err_code = 0;
	(*utils)->syntax_err = false;
	(*utils)->was_prev_line_null = 0;
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
		hist_fd = open("/tmp/.mshell_hist", O_RDWR | O_APPEND, 0644);
		while (1)
		{
			// find the newline number 10 from the bottom
			// and then read the amount of data until that
			err_check = get_next_line_nonewline(&gnl_line, hist_fd);
			if (!gnl_line && err_check == 0)
				break;
			else if (!gnl_line && err_check == 1)
				malloc_error(1);
			add_history(gnl_line);
			free (gnl_line);
			gnl_line = NULL;
		}
	}
	return (hist_fd);
}

char *rl_gets(char *line_read, int hist_file, t_utils *utils)
{
	int	savestdio;

	savestdio = dup(STDIN_FILENO);
	if (line_read)
	{
		free (line_read);
		line_read = NULL;
	}
	g_signal = 0;
	line_read = readline("MINISHELL-0.7$ ");
	if (!line_read && g_signal != 130)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		exit (utils->err_code);
	}
	else if (!line_read && g_signal == 130)
	{
		dup2 (savestdio, STDIN_FILENO);
		if (utils->was_prev_line_null == 0)
			ft_putchar_fd('\n', STDIN_FILENO);
		utils->was_prev_line_null = 1;
	}
	if (line_read && *line_read && g_signal != 130)
	{
		add_history(line_read);
		ft_putendl_fd(line_read, hist_file);
		utils->was_prev_line_null = 0;
	}
	close (savestdio);
	return (line_read);
}
