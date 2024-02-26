/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:44:03 by clundber          #+#    #+#             */
/*   Updated: 2024/02/26 16:50:39 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* void	initiate_node(t_cmd *cmd_node)

{


	
} */

char	*get_token(char *argv)

{
	static int	start;
	int			end;

	if (!start)
		start = 0;
	while (argv[start + end])
	{
		while (argv[start] == ' ' && argv[start]) // add all other whitespace
			start++;
		end = start;
		while (argv[end] != ' ' && argv[end]) // add all other whitespace
			end++;
		return (ft_substr(argv, start, end - start));
	}
	return (0);
}

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
		return (0);
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
		{
			ft_arrfree(new_array);
			return (0);
		}
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

void	error_func(char *str)

{
	ft_putendl_fd(str, 2);
	exit (1);
}

char	**lexer(char *argv)

{
	char	**temp_array;
	char	**temp_array2;
	int		i;
	int		array_size;

	array_size = 1;
	i = 0;
	temp_array2 = NULL;
	temp_array = NULL;
	while (1)
	{
		temp_array = malloc(sizeof(char *) * ((array_size * 20) +1));
		if (!temp_array)
			error_func("malloc failure\n");
		if (temp_array2)
			temp_array = array_copy(temp_array2);
		if (!temp_array)
			error_func("malloc failure\n");
		while (i < 20 * array_size)
		{
			temp_array[i] = get_token(argv);
			if (temp_array[i] == NULL)
				break ;
			i++;
		}
		array_size++;
		temp_array2 = array_copy(temp_array);
		ft_arrfree (temp_array);
	}
	return (temp_array);
}

int	main(int argc, char *argv[])

{
	t_cmd	*cmd_head;
	char	**array;

	array = NULL;
	cmd_head = NULL;
	array = lexer(argv[1]);

	int	i = 0;
	while (array[i])
		printf("%s\n", array[i++]);
}
