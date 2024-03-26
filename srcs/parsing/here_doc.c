/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 11:06:47 by clundber          #+#    #+#             */
/*   Updated: 2024/03/26 12:05:29 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	here_doc(t_pipe ***pipe)
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
				here_doc_open((*pipe)[x]->args[i], (*pipe)[x]);
				close ((*pipe)[x]->hd_fd[0]);
			}
			else if ((*pipe)[x]->tokens[i] == IN_HD)
				here_doc_open((*pipe)[x]->args[i], (*pipe)[x]);
			i++;
		}
		x++;
	}
}


void	here_doc_open(char *eof, t_pipe *_pipe)
{
	char	*buff;

	pipe(_pipe->hd_fd);
	while (1)
	{
		buff = readline("> ");
		if (!buff)
			break ;
		if (ft_strncmp(eof, buff, -1) == 0)
			break ;
		ft_putendl_fd(buff, _pipe->hd_fd[1]);
		free (buff);
	}
	close(_pipe->hd_fd[1]);
	free(buff);
}
