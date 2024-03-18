/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:44:03 by clundber          #+#    #+#             */
/*   Updated: 2024/03/18 16:51:17 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**array_copy(char **array)

{
	char	**new_array;
	int		i;

	i = 0;
	new_array = NULL;
	while (array[i])
		i++;
	new_array = malloc(sizeof(char *) * (i +1));
	if (!new_array)
		malloc_error(1);
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
		{
			ft_arrfree(new_array);
			malloc_error(1);
		}
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

char	**get_cmd(char **cmds, int start, int end)

{
	char	**temp_arr;
	int		i;

	i = 0;
	temp_arr = NULL;
	temp_arr = malloc(sizeof (char *) * ((end - start) + 2));
	if (!temp_arr)
		malloc_error(1);
	while (cmds[start] && start <= end)
	{
		temp_arr[i] = ft_strdup(cmds[start]);
		if (!temp_arr[i])
			malloc_error(1);
		i++;
		start++;
	}
	temp_arr[i] = NULL;
	return (temp_arr);
}

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

void	pre_parse(char **array, t_pipe ***pipe)

{
	int	i;
	int	x;
	int	start;

	i = 0;
	x = 0;
	start = 0;
	while (array[i])
	{
		if (array[i][0] == '|')
		{
			(*pipe)[x]->args = get_cmd(array, start, i -1);
			x++;
			start = i +1;
		}
		else if (array[i +1] == 0)
		{
			(*pipe)[x]->args = get_cmd(array, start, i);
			break ;
		}
		i++;
	}
}

void	malloc_error(int err)

{
	ft_putendl_fd("Malloc failed\n", 2);
	exit(err);
}

int	line_checker(char **str, int *err_code)
{
	int	i;

	i = 0;
	while ((*str)[i] == ' ')
		i++;
	if ((*str)[i] == '|')
		{
			ft_putendl_fd("syntax error near unexpected token `|'", 2);
			*err_code = 258;
			return (1);
		}
	return (0);
}


int	lexer(char *line_read, t_pipe ***pipe, t_utils *utils)
{
	char	**array;

	array = NULL;
	if (var_substitution(&line_read, utils->envp, utils->err_code) == 1)
		return (1);
	if (!line_read || !line_read[0])
		return(1);
	if (line_checker(&line_read, &utils->err_code) == 1)
		return (1);
	array = ms_split(line_read);
	free(line_read);
	if (!array)
		return (1); // or  something else if malloc failed
/*  	int	i = 0;
	while (array[i])
	{
		printf("array = %s\n", array[i]);
	 	i++;
	}   
	printf("------------\n"); */
	pipeline_init(array, pipe); 
	if (parser(array, pipe, &utils->err_code) == 1)
		return (1); // free all first
	ft_arrfree(array);
/* 	int i = 0;
	int x = 0;
	while ((*pipe)[i])
	{
		x = 0;
		while ((*pipe)[i]->args[x])
		{
			printf("args = %s\n", (*pipe)[i]->args[x]);
			printf("token = %d\n", (*pipe)[i]->tokens[x]);
			x++;
		}
		i++;
	} */
	

	return (0);
}
