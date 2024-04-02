/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 11:06:47 by clundber          #+#    #+#             */
/*   Updated: 2024/04/02 16:32:37 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	here_doc(t_pipe ***pipe, t_utils *utils)
{
	int	i;
	int	x;

	x = 0;
	while ((*pipe)[x])
	{
		i = 0;
		while ((*pipe)[x]->args[i])
		{
			if ((*pipe)[x]->tokens[i] == SKIP_HD)
			{
				here_doc_open((*pipe)[x]->args[i], (*pipe)[x], utils);
				close ((*pipe)[x]->hd_fd[0]);
			}
			else if ((*pipe)[x]->tokens[i] == IN_HD)
				here_doc_open((*pipe)[x]->args[i], (*pipe)[x], utils);
			if (g_signal == 130)
					return ;
			i++;
		}
		x++;
	}
}


void	here_doc_open(char *eof, t_pipe *_pipe, t_utils *utils)
{
	char	*buff;
	int save_stdin;

	save_stdin = dup(STDIN_FILENO);
	pipe(_pipe->hd_fd);
	while (1)
	{
		buff = readline("> ");
		if (g_signal == 130)
		{
			dup2 (save_stdin, STDIN_FILENO);
			close (save_stdin);
			//ft_putstr_fd("\b\b\033[K", STDOUT_FILENO);
			ft_putendl_fd(">", STDOUT_FILENO);
			utils->was_prev_line_null = 1;
			break;
		}
		if (!buff)
			break ;
		if (ft_strncmp(eof, buff, -1) == 0)
			break ;
		ft_putendl_fd(buff, _pipe->hd_fd[1]);
		free (buff);
	}
	close(_pipe->hd_fd[1]);
	close (save_stdin);
	free(buff);
}
