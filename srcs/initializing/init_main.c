/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:55:30 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/23 16:34:30 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	g_signal;

int	main(int ac, char **av, char **sys_envp)
{
	int		ret;

	(void)ac;
	(void)av;
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
	ms.pipe = NULL;
	intialize_utils(sys_envp, &ms.utils, &ms);
	while (1)
	{
		g_signal = 0;
		ms.line = rl_gets(ms.line, ms.utils, &ms);
		if (ms.line && parsing(&ms) != 1)
		{
			i = -1;
			while (ms.pipe[++i])
				ms.pipe[i]->cmd_with_path = \
				assign_scmd_path(ms.pipe[i]->noio_args[0], ms.utils->envp, &ms);
			ms.utils->err_code = execute(ms.utils, ms.pipe, &ms);
		}
		free_and_exit(&ms.pipe, NULL, NULL, -42);
	}
	free_and_exit(NULL, &ms.utils, &ms, -42);
	return (0);
}

static void	rl_gets_error(t_utils *utils)
{
	ft_putendl_fd("exit", STDOUT_FILENO);
	exit (utils->err_code);
}

char	*rl_gets(char *line_read, t_utils *utils, t_ms *ms)
{
	int	savestdio;

	savestdio = dup_and_check(STDIN_FILENO, ms);
	if (line_read)
		ft_nullfree(&line_read);
	g_signal = 0;
	handle_sigquit(true);
	line_read = readline(YEL"MINISHELL-1.01$ "CRESET);
	handle_sigquit(false);
	if (!line_read && g_signal == 0)
		rl_gets_error(utils);
	else if (!line_read && g_signal != 0)
	{
		dup2_and_check(savestdio, STDIN_FILENO, ms);
		utils->err_code = 1;
		if (utils->was_prev_line_null == 0)
			ft_putchar_fd('\n', STDIN_FILENO);
		utils->was_prev_line_null = 1;
	}
	if (line_read && *line_read && g_signal == 0)
	{
		add_history(line_read);
		utils->was_prev_line_null = 0;
	}
	close (savestdio);
	return (line_read);
}
