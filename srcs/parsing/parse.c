/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 19:03:25 by clundber          #+#    #+#             */
/*   Updated: 2024/03/04 12:56:17 by klukiano         ###   ########.fr       */
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
		error_func("Token malloc failure\n");
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

void	make_tokens(t_pipe *pipe)

{
	int	i;
	int	x;

	i = 0;
	// still need freeing function upon error
	while (pipe->args[i])
	{
		if (ft_strncmp(pipe->args[i],"<", 2) == 0)
		{
			pipe->tokens[i] = REMOVE;
			if (pipe->tokens[i +1] == CMD)
				pipe->tokens[i +1] = IN_FD;
			else
				error_func("syntax error near unexpected token 'insert token here'");
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
		else if (ft_strncmp(pipe->args[i],"<<", 3) == 0)
		{
			pipe->tokens[i] = REMOVE;
			if (pipe->tokens[i +1] == CMD)
				pipe->tokens[i +1] = IN_HD;
			else
				error_func("syntax error near unexpected token 'insert token here'");
			while (x >= 0)
			{
				if (pipe->tokens[x] == IN_FD)
					pipe->tokens[x] = SKIP_IN;
				if (pipe->tokens[x] == IN_HD)
					pipe->tokens[x] = SKIP_HD;
				x--;
			}
		}
		else if (ft_strncmp(pipe->args[i],">", 2) == 0)
		{
			pipe->tokens[i] = REMOVE;
			if (pipe->tokens[i +1] == CMD)
				pipe->tokens[i +1] = OUT;
			else
				error_func("syntax error near unexpected token 'insert token here'");
			x = (i -1);
			while (x >= 0)
			{
				if (pipe->tokens[x] == OUT || pipe->tokens[x] == OUT_AP)
					pipe->tokens[x] = SKIP_OUT;
				x--;
			}
		}
		else if (ft_strncmp(pipe->args[i],">>", 3) == 0)
		{
			pipe->tokens[i] = REMOVE;
			if (pipe->tokens[i +1] == CMD)
				pipe->tokens[i +1] = OUT_AP;
			else
				error_func("syntax error near unexpected token 'insert token here'");
			x = (i -1);
			while (x >= 0)
			{
				if (pipe->tokens[x] == OUT || pipe->tokens[x] == OUT_AP)
					pipe->tokens[x] = SKIP_OUT;
				x--;
			}
		}
		i++;
	}

}

void	remove_red(t_pipe *pipe)

{
	int		i;
	int		x;
	char	**temp;
	int		*i_temp;

	i = 0;
	x = 1;
	while (pipe->tokens[i] != 0)
	{
		if (pipe->tokens[i] != REMOVE)
			x++;
		i++;
	}
	temp = malloc(sizeof(char *) * (x +1));
	i_temp = malloc(sizeof(int *) * (x +1));
	x = 0;
	i = 0;
	while (pipe->args[i])
	{
		if (pipe->tokens[i] != REMOVE)
		{
			temp[x] = ft_strdup(pipe->args[i]);
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

void	parser(char **array, t_pipe ***pipe)

{
	int	x;
	int	i;

	x = 0;
	i = 0;
	pre_parse(array, pipe);
	while ((*pipe)[x])
	{
		init_token((*pipe)[x]);
		make_tokens((*pipe)[x]);
		remove_red((*pipe)[x]);
		x++;
	}

}
