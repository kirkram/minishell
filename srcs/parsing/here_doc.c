/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 11:06:47 by clundber          #+#    #+#             */
/*   Updated: 2024/04/17 18:29:18 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	here_doc(t_pipe ***pipe, t_utils *utils, t_ms *ms)
{
	int	i;
	int	x;

	x = 0;
	while ((*pipe)[x])
	{
		i = 0;
		while ((*pipe)[x]->args[i])
		{
			handle_sigquit(true);
			if ((*pipe)[x]->tokens[i] == SKIP_HD)
			{
				here_doc_open((*pipe)[x]->args[i], (*pipe)[x], utils, ms);
				close ((*pipe)[x]->hd_fd[0]);
			}
			else if ((*pipe)[x]->tokens[i] == IN_HD)
				here_doc_open((*pipe)[x]->args[i], (*pipe)[x], utils, ms);
			handle_sigquit(false);
			if (g_signal != 0)
				return ;
			i++;
		}
		x++;
	}
}

void	hd_env(char **buff, t_ms *ms)
{
	bool	quote;
	bool	dquote;

	quote = false;
	dquote = false;
	env_variable(ms, quote, dquote, buff);
}

void	here_doc_open(char *eof, t_pipe *_pipe, t_utils *utils, t_ms *ms)
{
	char	*buff;
	int		save_stdin;

	save_stdin = dup(STDIN_FILENO);
	pipe(_pipe->hd_fd);
	while (1)
	{
		buff = readline("> ");
		if (g_signal != 0)
		{
			dup2 (save_stdin, STDIN_FILENO);
			close (save_stdin);
			ft_putendl_fd(">", STDOUT_FILENO);
			utils->was_prev_line_null = 1;
			break ;
		}
		if (!buff || ft_strncmp(eof, buff, -1) == 0)
			break ;
		hd_env(&buff, ms);
		ft_putendl_fd(buff, _pipe->hd_fd[1]);
		free (buff);
	}
	close(_pipe->hd_fd[1]);
	close (save_stdin);
	free(buff);
}
