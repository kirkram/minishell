/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 19:03:25 by clundber          #+#    #+#             */
/*   Updated: 2024/03/26 16:30:08 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_token(t_pipe *pipe)
{
	int	i;

	i = 0;
	while (pipe->args[i])
		i++;
	pipe->tokens = malloc (sizeof(int *) * (i +2));
	if (!pipe->tokens)
		malloc_error(1);
	i = 0;
	while (pipe->args[i])
	{
		if (get_token(pipe->args[i]) == CMD)
			pipe->tokens[i] = CMD;
		else
			pipe->tokens[i] = 0;
		i++;
	}
	pipe->tokens[i] = 0;
}


void	remove_in(t_pipe *pipe, int i, int hd)
{
	int	x;

	x = (i -1);
	pipe->tokens[i] = REMOVE;
	if (pipe->tokens[i +1] == CMD)
	{
		if (hd == 0)
			pipe->tokens[i +1] = IN_FD;
		else
			pipe->tokens[i +1] = IN_HD;
	}
	x = (i -1);
	while (x >= 0)
	{
		if (pipe->tokens[x] == IN_FD)
			pipe->tokens[x] = SKIP_IN;
		if (pipe->tokens[x] == IN_HD)
			pipe->tokens[x] = SKIP_HD;
		x--;
	}
}

void	remove_out(t_pipe *pipe, int i, int app)
{
	int	x;

	x = (i -1);
	pipe->tokens[i] = REMOVE;
	if (pipe->tokens[i +1] == CMD)
	{
		if (app == 0)
			pipe->tokens[i +1] = OUT;
		else
			pipe->tokens[x] = SKIP_OUT;
	}
	x = (i -1);
	while (x >= 0)
	{
		if (pipe->tokens[x] == OUT || pipe->tokens[x] == OUT_AP)
			pipe->tokens[x] = SKIP_OUT;
		x--;
	}
}

int	make_tokens(t_pipe *pipe, int i)
{
	while (pipe->args[i])
	{
		if (ft_strncmp(pipe->args[i], "<", 2) == 0)
			remove_in(pipe, i, 0);
		else if (ft_strncmp(pipe->args[i], "<<", 3) == 0)
			remove_in(pipe, i, 1);
		else if (ft_strncmp(pipe->args[i], ">", 2) == 0)
			remove_out(pipe, i, 0);
		else if (ft_strncmp(pipe->args[i], ">>", 3) == 0)
			remove_out(pipe, i, 1);
		i++;
	}
	return (0);
}

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
		if (pipe->tokens[i] == IN_FD || pipe->tokens[i] == IN_HD)
			pipe->infile = ft_strdup(pipe->args[i]);
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
