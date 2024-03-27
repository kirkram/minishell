/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 18:58:14 by clundber          #+#    #+#             */
/*   Updated: 2024/03/27 19:08:43 by clundber         ###   ########.fr       */
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
			pipe->tokens[i +1] = OUT_AP;
	}
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
