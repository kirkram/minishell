/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:55:30 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/04 12:05:47 by clundber         ###   ########.fr       */
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
	char	*line_read;
	t_pipe	**_pipe;
	int		i;
	t_utils	*utils;

	intialize_utils(sys_envp, &utils);
	line_read = NULL;

	while (1)
	{
		g_signal = 0;
		utils->syntax_err = false;
		line_read = rl_gets(line_read, hist_fd, utils->err_code, utils);
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
			lvl = ft_atoi(ft_substr(str, start, i - start));
			lvl++;
			return (ft_strjoin(ft_substr(str, 0, start), ft_itoa(lvl)));
		}
		i++;
	}
	return (ft_strdup(str));

}

void	intialize_utils(char **sys_envp, t_utils **utils)
{
	int		i;
	//char	cwd_buf[4096];

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
			if (!(*utils)->envp[i])
				malloc_error(1);
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
	int		len;

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
				ft_putendl_fd("GNL History file Critical Failure", 2);
				malloc_error(1);
			}
			len = ft_strlen(gnl_line) - 1;
			if (gnl_line[len] == '\n')
				gnl_line[len] = '\0';
			add_history(gnl_line);
			free (gnl_line);
			gnl_line = NULL;
		}
	}
	return (hist_fd);
}

char *rl_gets(char *line_read, int hist_file, int err_code, t_utils *utils)
{
	int	savestdio;

	savestdio = dup(STDIN_FILENO);
	if (line_read)
	{
		free (line_read);
		line_read = NULL;
	}
	g_signal = 0;
	//ft_putendl_fd("g_signal = 0", 2);
	line_read = readline("MINISHELL-0.7$ ");
	if (!line_read && g_signal != 130)
	{
		//only works with ctrl + d?
		ft_putendl_fd("exit", STDOUT_FILENO);
		exit (err_code);
	}
	else if (!line_read && g_signal == 130)
	{
		dup2 (savestdio, STDIN_FILENO);
		//ft_putstr_fd("\b\b\033[K", STDOUT_FILENO);
		//if (!was_previous_empty)
		// ..rl_on_new_line();
		if (utils->was_prev_line_null == 0)
			ft_putchar_fd('\n', STDIN_FILENO);
		ft_putstr_fd("\b\b\033[K", STDOUT_FILENO);
		utils->was_prev_line_null = 1;
		//line_read = ft_strdup("\n");
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
