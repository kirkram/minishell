/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 19:15:47 by clundber          #+#    #+#             */
/*   Updated: 2024/03/27 19:48:54 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	pipeline_init(char **array, t_pipe ***pipe)
{
	int	i;
	int	x;

	i = 0;
	x = 1;
	while (array[i])
	{
		if (array[i][0] == '|')
			x++;
		i++;
	}
	*pipe = malloc(sizeof(t_pipe *) * (x +1));
	if (!*pipe)
		malloc_error(1);
	i = 0;
	while (i < x)
	{
		(*pipe)[i] = malloc(sizeof(t_pipe));
		if (!(*pipe)[i])
			malloc_error(1);
		i++;
	}
	(*pipe)[x] = NULL;
}

void	init_tokenarr(int **tokens, char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	(*tokens) = malloc (sizeof(int *) * (i +2));
	if (!(*tokens))
		malloc_error(1);
	i = 0;
	while (array[i])
	{
		(*tokens)[i] = get_token(array[i]);
		i++;
	}
	(*tokens)[i] = 0;
}
