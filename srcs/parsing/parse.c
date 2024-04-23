/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 19:03:25 by clundber          #+#    #+#             */
/*   Updated: 2024/04/23 17:53:17 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_remove_red(t_pipe *pipe, char ***temp, int **i_temp, t_ms *ms)
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
		malloc_check(NULL, ms);
	(*i_temp) = malloc(sizeof(int *) * (x +1));
	if (!(*i_temp))
		malloc_check(NULL, ms);
}

void	remove_red(t_pipe *pipe, int i, t_ms *ms)
{
	int		x;
	char	**temp;
	int		*i_temp;

	x = 0;
	i = 0;
	init_remove_red(pipe, &temp, &i_temp, ms);
	while (pipe->args[i])
	{
		if (pipe->tokens[i] != REMOVE)
		{
			temp[x] = ft_strdup(pipe->args[i]);
			if (!temp[x])
				malloc_check(NULL, ms);
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

void	init_noio(t_pipe *pipe, t_ms *ms)
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
		malloc_check(NULL, ms);
}

void	final_args(t_pipe *pipe, int i, t_ms *ms)
{
	int	x;

	x = 0;
	init_noio(pipe, ms);
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
				malloc_check(NULL, ms);
			x++;
		}
		i++;
	}
	pipe->noio_args[x] = NULL;
}

void	parser(char **array, t_pipe ***pipe, t_ms *ms)
{
	int	x;
	int	i;

	i = 0;
	x = 0;
	pre_parse(array, pipe, ms);
	ft_arrfree(array);
	while ((*pipe)[x])
	{
		(*pipe)[x]->cmd_with_path = NULL;
		init_token((*pipe)[x], ms);
		make_tokens((*pipe)[x], i);
		quote_remover((*pipe)[x], ms);
		remove_red((*pipe)[x], i, ms);
		final_args((*pipe)[x], i, ms);
		(*pipe)[x]->hd_fd[0] = -1;
		(*pipe)[x]->hd_fd[1] = -2;
		x++;
	}
}
