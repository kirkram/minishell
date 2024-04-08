/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 19:03:25 by clundber          #+#    #+#             */
/*   Updated: 2024/04/05 15:53:03 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_remove_red(t_pipe *pipe, char ***temp, int **i_temp)
{
	int	i;
	int	x;

	i = 0;
	x = 1;
	while (pipe->tokens[i] != 0)
	{
		if (pipe->tokens[i] != REMOVE)
			x++;
		i++;
	}
	(*temp) = malloc(sizeof(char *) * (x +1));
	if (!(*temp))
		malloc_error(1);
	(*i_temp) = malloc(sizeof(int *) * (x +1));
	if (!(*i_temp))
		malloc_error(1);
}

void	remove_red(t_pipe *pipe, int i)
{
	int		x;
	char	**temp;
	int		*i_temp;

	x = 0;
	i = 0;
	init_remove_red(pipe, &temp, &i_temp);
	while (pipe->args[i])
	{
		if (pipe->tokens[i] != REMOVE)
		{
			temp[x] = ft_strdup(pipe->args[i]);
			if (!temp[x])
				malloc_error (1);
			i_temp[x] = pipe->tokens[i];
			x++;
		}
		i++;
	}
	temp[x] = NULL;
	i_temp[x] = 0;
	ft_arrfree(pipe->args);
	free(pipe->tokens);
	pipe->args = temp;
	pipe->tokens = i_temp;
}

void	init_noio(t_pipe *pipe)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (pipe->args[i])
	{
		if (pipe->tokens[i] == CMD)
			x++;
		i++;
	}
	pipe->noio_args = malloc (sizeof(char *) * (x +1));
	if (!pipe->noio_args)
		malloc_error(1);
}

int	final_args(t_pipe *pipe, int i)
{
	int	x;

	x = 0;
	init_noio(pipe);
	while (pipe->args[i])
	{
		if (pipe->tokens[i] == CMD)
		{
			pipe->noio_args[x] = ft_strdup(pipe->args[i]);
			if (x == 0)
			{
				if (is_builtin(pipe->args[i]) == 8)
					pipe->tokens[i] = 8;
			}
			if (!pipe->noio_args[x])
				malloc_error(1);
			x++;
		}
		i++;
	}
	pipe->noio_args[x] = NULL;
	return (0);
}

int	parser(char **array, t_pipe ***pipe, int *err_code)
{
	int	x;
	int	i;

	i = 0;
	x = 0;
	pre_parse(array, pipe);
	while ((*pipe)[x])
	{
		init_token((*pipe)[x]);
		if (make_tokens((*pipe)[x], i) == 1)
			return (1);
		quote_remover((*pipe)[x]);
		remove_red((*pipe)[x], i);
		if (final_args((*pipe)[x], i) == 1)
		{
			*err_code = 1;
			return (1);
		}
		x++;
	}
	return (0);
}
