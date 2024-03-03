/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:44:03 by clundber          #+#    #+#             */
/*   Updated: 2024/03/03 14:56:17 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "parsing.h"

void	error_func(char *str)

{
	ft_putendl_fd(str, 2);
	exit (1);
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
		error_func("Malloc failure\n");
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
		{
			ft_arrfree(new_array);
			error_func("Malloc failure\n");
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
		error_func("Malloc failure\n");
	while (cmds[start] && start <= end)
	{
		temp_arr[i] = ft_strdup(cmds[start]);
		if (!temp_arr[i])
			error_func("Malloc failure\n");
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
		error_func("malloc failed\n");
 	i = 0;
	while (i < x)
	{
		(*pipe)[i] = malloc(sizeof(t_pipe));
		if (!(*pipe)[i])
			error_func("malloc failed\n");		
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
			break;
		}
		i++;
	}
}

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
	temp[x] = '\0';
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

void	lexer(char *argv, char **envp)

{
	t_pipe	**pipe;
	char	**array;

	array = NULL;
	array = ppx_split(argv, ' ');
	if (!array)
		error_func("no valid argument\n");
	var_substitution(array, envp);
	pipeline_init(array, &pipe);
	parser(array, &pipe);
	ft_arrfree(array);

	// NOT PART OF THE ACCTUAL FUNCTION //

 	int	i = 0;
	int	x = 0;	
	while (pipe[i])
	{
		x = 0;
		while (pipe[i]->args[x])
		{
			printf("%s     ", pipe[i]->args[x]);
			printf("%d\n", pipe[i]->tokens[x]);
			x++;
		}
		printf("------------\n");
		i++;
	} 
	i = 0;
	while (pipe[i])
	{
		ft_arrfree(pipe[i]->args);
		free (pipe[i]->tokens);
		free(pipe[i]);
		i++;
	}
	free (pipe);
}
