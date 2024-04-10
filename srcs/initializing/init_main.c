/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:55:30 by klukiano          #+#    #+#             */

/*   Updated: 2024/04/10 13:37:31 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../lib/get_next_line/get_next_line.h"

int	g_signal;

int	main(int ac, char **av, char **sys_envp)
{

	(void)ac;
	(void)av;
	int		ret;
	g_signal = 0;

	signal_handler();
	ret = 0;
	ret = interactive_mode_loop(sys_envp);
	return (ret);
}

int	interactive_mode_loop(char **sys_envp)
{
	int		i;
	t_ms	ms;

	ms.line = NULL;
	ms.temp = NULL;
	ms.temp2 = NULL;
	ft_bzero(ms.pipe, sizeof(t_pipe));
	ms.pipe = NULL;
	intialize_utils(sys_envp, &ms.utils, &ms);
	while (1)
	{
		g_signal = 0;
		ms.line = rl_gets(ms.line, ms.utils);
		if (ms.line && parsing(&ms) != 1)
		{
			i = -1;
			while (ms.pipe[++i])
				ms.pipe[i]->cmd_with_path = assign_scmd_path(ms.pipe[i]->noio_args[0], ms.utils->envp);
			ms.utils->err_code = execute(ms.utils, ms.pipe, &ms);
		}
		free_pipes_utils_and_exit(&ms.pipe, NULL, -42);
	}
	free (ms.line);
	free_pipes_utils_and_exit(NULL, &ms.utils, -42);
	return (0);
}

char *exp_init(char *str1, char *str2, t_ms *ms)
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
	malloc_check(&temp, ms);
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

char	*shell_level(char *str, t_ms *ms)
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
			malloc_check(&temp, ms);
			lvl = ft_atoi(temp);
			ft_nullfree(&temp);
			lvl++;
			return (ft_free_strjoin(ft_substr(str, 0, start), ft_itoa(lvl)));
		}
		i++;
	}
	return (ft_strdup(str));
}

void	intialize_utils(char **sys_envp, t_utils **utils, t_ms *ms)
{
	int		i;

	*(utils) = malloc(sizeof(t_utils));
	if (*utils == NULL)
		malloc_check(NULL, ms);
	i = 0;
	while (sys_envp[i])
		i ++;
	(*utils)->envp = malloc((i + 1) * sizeof(char *));
	if ((*utils)->envp == NULL)
		malloc_check(NULL, ms);
	(*utils)->export = malloc((i + 1) * sizeof(char *));
	if ((*utils)->export == NULL)
		malloc_check(NULL, ms);
	i = 0;
	while (sys_envp[i])
	{
 		if (ft_strncmp("SHELL=", sys_envp[i], 6) == 0)
		{
			(*utils)->envp[i] = ft_strjoin("SHELL=", "minishell");
			malloc_check(&(*utils)->envp[i], ms);
			(*utils)->export[i] = exp_init("declare -x ", (*utils)->envp[i], ms);
		}
		else if (ft_strncmp("SHLVL=", sys_envp[i], 6) == 0)
		{
			(*utils)->envp[i] = shell_level(sys_envp[i], ms);
			malloc_check(&(*utils)->envp[i], ms);
			(*utils)->export[i] = exp_init("declare -x ", (*utils)->envp[i], ms);
		}
		else
		{
			(*utils)->envp[i] = ft_strdup(sys_envp[i]);
			malloc_check(&(*utils)->envp[i], ms);
			(*utils)->export[i] = exp_init("declare -x ", sys_envp[i], ms);
		}
		i ++;
	}
	(*utils)->syntax_err = false;
	(*utils)->was_prev_line_null = false;
	(*utils)->envp[i] = NULL;
	(*utils)->export[i] = NULL;
	(*utils)->err_code = 0;
	sort_export(*utils);
}

char *rl_gets(char *line_read, t_utils *utils)
{
	int	savestdio;

	savestdio = dup(STDIN_FILENO);
	if (line_read)
	{
		free (line_read);
		line_read = NULL;
	}
	g_signal = 0;
	line_read = readline(YEL"MINISHELL-0.7$ "CRESET);
	if (!
	line_read && g_signal != 130)
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
		utils->was_prev_line_null = 0;
	}
	close (savestdio);
	return (line_read);
}
