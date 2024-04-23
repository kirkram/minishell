/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 14:55:11 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/23 15:14:19 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	dup_and_check(int dup_fd, t_ms *ms)
{
	int	ret;

	ret = 0;
	ret = dup(dup_fd);
	if (ret < 0)
	{
		ft_putendl_fd("error in dup", 2);
		free_and_exit(&ms->pipe, &ms->utils, ms, 1);
	}
	return (ret);
}

void	dup2_and_check(int fd_over, int fd_replaced, t_ms *ms)
{
	if (fd_over < 0 || fd_replaced < 0)
		return ;
	if (dup2(fd_over, fd_replaced) < 0)
	{
		ft_putendl_fd("dup2 error", 2);
		free_and_exit(&ms->pipe, &ms->utils, ms, 1);
	}
}
