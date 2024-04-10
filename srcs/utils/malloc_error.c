/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:55:33 by clundber          #+#    #+#             */
/*   Updated: 2024/04/10 19:11:13 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	malloc_check(char **str, t_ms *ms)
{
	int	i;

	i = 0;
	if ((str && *str))
		return ;
	if (ms->pipe)
	{
		while (ms->pipe[i])
		{
			ft_arrfree(ms->pipe[i]->args);
			ft_arrfree(ms->pipe[i]->noio_args);
			free(ms->pipe[i]->tokens);
			free(ms->pipe[i]->cmd_with_path);
			free(ms->pipe[i]);
			i++;
		}
		if (ms->pipe[i])
			free(ms->pipe[i]);
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

void ft_nullfree(char **str)
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