/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:44:03 by clundber          #+#    #+#             */
/*   Updated: 2024/03/12 17:03:52 by klukiano         ###   ########.fr       */
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
	{
		error_func("malloc failed\n");
		exit (1);
	}
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
		{
			ft_arrfree(new_array);
			error_func("malloc failed\n");
			exit (1);
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
	{
		error_func("malloc failed\n");
		exit (1);
	}
	while (cmds[start] && start <= end)
	{
		temp_arr[i] = ft_strdup(cmds[start]);
		if (!temp_arr[i])
		{
			error_func("malloc failed\n");
			exit (1);
		}
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
	{
		error_func("malloc failed\n");
		exit (1);
	}
	i = 0;
	while (i < x)
	{
		(*pipe)[i] = malloc(sizeof(t_pipe));
		if (!(*pipe)[i])
		{
			error_func("malloc failed\n");
			exit (1);
		}
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

void	error_func(char *str)

{
	ft_putendl_fd(str, 2);
}


int			lexer(char *line_read, t_pipe ***pipe, t_utils *utils)

{
	char	**array;

	array = NULL;
	var_substitution(&line_read, utils->envp);
	array = ppx_split(line_read, ' ');
	if (!array)
		return (1); // or  something else if malloc failed
	pipeline_init(array, pipe);
	if (parser(array, pipe, &utils->err_code) == 1)
		return (1); // free all first
	ft_arrfree(array);

	// NOT PART OF THE ACCTUAL FUNCTION //

     int	i = 0;
	 int	x = 0;
	 while ((*pipe)[i])
	 {
	 	x = 0;
	 	while ((*pipe)[i]->noio_args[x])
	 	{
	 		//printf("%s\n", (*pipe)[i]->noio_args[x]);
	 		//printf("%d\n", (*pipe)[i]->tokens[x]);
	 		x++;
	 	}
	 	//printf("------------\n");
	 	i++;
	 }
/*
	 int	i = 0;
	 int	x = 0;
	 while ((*pipe)[i])
	 {
	 	x = 0;
	 	while ((*pipe)[i]->args[x])
	 	{
	 		printf("%s        ", (*pipe)[i]->args[x]);
	 		printf("%d\n", (*pipe)[i]->tokens[x]);
	 		x++;
	 	}
	 	printf("------------\n");
	 	i++;
	 }  */
/* 	 int x = 0;
 	 int i = 0;
	 while ((*pipe)[i])
	 {
		while (pipe[i]->args[x])
	 		ft_arrfree(pipe[i]->args[x]);
	 	free (pipe[i]->tokens);
	 	free(pipe[i]);
	 	i++;
	 }  */
	// free (pipe);

	return (0);
}
