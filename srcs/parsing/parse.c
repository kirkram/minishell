/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 19:03:25 by clundber          #+#    #+#             */
/*   Updated: 2024/03/06 23:06:46 by clundber         ###   ########.fr       */
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
	{
		error_func("malloc failed\n");
		exit (1);
	}
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

int	make_tokens(t_pipe *pipe, int *err_code)

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
			{
				*err_code = 258;
				ft_putstr_fd("syntax error near unexpected token `", 2);
				ft_putstr_fd(pipe->args[i +1], 2);
				ft_putendl_fd("\'", 2);
				return (1);
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
		else if (ft_strncmp(pipe->args[i],"<<", 3) == 0)
		{
			pipe->tokens[i] = REMOVE;
			if (pipe->tokens[i +1] == CMD)
				pipe->tokens[i +1] = IN_HD;
			else
			{
				*err_code = 258;
				ft_putstr_fd("syntax error near unexpected token `", 2);
				ft_putstr_fd(pipe->args[i +1], 2);
				ft_putendl_fd("\'", 2);
				return (1);
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
		else if (ft_strncmp(pipe->args[i],">", 2) == 0)
		{
			pipe->tokens[i] = REMOVE;
			if (pipe->tokens[i +1] == CMD)
				pipe->tokens[i +1] = OUT;
			else
			{
				*err_code = 258;
				ft_putstr_fd("syntax error near unexpected token `", 2);
				ft_putstr_fd(pipe->args[i +1], 2);
				ft_putendl_fd("\'", 2);
				return (1);
			}
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
			{
				*err_code = 258;
				ft_putstr_fd("syntax error near unexpected token `", 2);
				ft_putstr_fd(pipe->args[i +1], 2);
				ft_putendl_fd("\'", 2);
				return (1);
			}
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
	return (0);
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

int	is_builtin(char *str)

{
	if (ft_strncmp(str, "echo", 5) == 0)
		return (8);
	else if (ft_strncmp(str, "cd", 3) == 0)
		return (8);
	else if (ft_strncmp(str, "pwd", 4) == 0)
		return (8);
	else if (ft_strncmp(str, "export", 7) == 0)
		return (8);
	else if (ft_strncmp(str, "unset", 6) == 0)
		return (8);
	else if (ft_strncmp(str, "env", 4) == 0)
		return (8);
	else if (ft_strncmp(str, "exit", 5) == 0)
		return (8);
	return (0);
}

int	final_args(t_pipe *pipe)

{
	int	i;
	int	x;

	x = 0;
	i = 0;
	while (pipe->args[i])
	{
		if (pipe->tokens[i] == CMD)
			x++;
		i++;
	}
	pipe->final_args = malloc (sizeof(char *) * (x +1));
	if (!pipe->final_args)
	{
		error_func("malloc failed\n");
		exit (1);
	}
	i = 0;
	x = 0;
	while (pipe->args[i])
	{
		if (pipe->tokens[i] == CMD)
		{
			pipe->final_args[x] = ft_strdup(pipe->args[i]);
			if (x == 0)
			{
				if (is_builtin(pipe->args[i]) == 8)
					pipe->tokens[i] = 8;
			}
			if (!pipe->final_args[x])
			{
				error_func("malloc failed\n");
				exit (1);
			}
			x++;
		}
		i++;
	}
	pipe->final_args[x] = NULL;
	return (0);
}

int	parser(char **array, t_pipe ***pipe, int *err_code)

{
	int	x;

	x = 0;
	pre_parse(array, pipe);
	while ((*pipe)[x])
	{
		init_token((*pipe)[x]);
		if (make_tokens((*pipe)[x], err_code) == 1)
			return (1);
		remove_red((*pipe)[x]);
		if (final_args((*pipe)[x]) == 1)
			return (1); 
		x++;
	}
	return (0);
}
