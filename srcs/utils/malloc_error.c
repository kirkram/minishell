/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:55:33 by clundber          #+#    #+#             */
/*   Updated: 2024/04/13 17:01:39 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	malloc_check(char **str, t_ms *ms)
{
	int	i;

	i = -1;
	if ((str && *str))
		return ;
	if (ms->pipe)
	{
		while (ms->pipe[++i])
			free_one_pipe(ms->pipe[i]);
		free (ms->pipe);
	}
	if (ms->utils)
	{
		ft_arrfree(ms->utils->envp);
		ft_arrfree(ms->utils->export);
		free(ms->utils);
	}
	if (ms->line)
		free (ms->line);
	if (ms->temp)
		free (ms->temp);
	if (ms->temp2)
		free (ms->temp2);
	ft_putendl_fd("minishell: malloc failed", 2);
	exit (1);
}

void	ft_nullfree(char **str)
{
	if (*str)
	{
		free (*str);
		(*str) = NULL;
	}
}

char	**free_reverse(int i, char **array)
{
	while (i >= 0)
	{
		free (array[i]);
		i --;
	}
	free (array);
	return (NULL);
}
