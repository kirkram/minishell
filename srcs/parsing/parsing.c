/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:44:03 by clundber          #+#    #+#             */
/*   Updated: 2024/03/22 15:43:49 by clundber         ###   ########.fr       */
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

void	init_tokenarr(int **tokens, char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	(*tokens) = malloc (sizeof(int *) * (i +2));
	if (!(*tokens))
		malloc_error(1);
	i = 0;
	while (array[i])
	{
		(*tokens)[i] = get_token(array[i]);
		i++;
	}
	(*tokens)[i] = 0;
}



int	syntax_check(int *tokens, int *err_code, char **array)
{
	int	i;

	i = 0;
	while (tokens[i] != 0)
	{
		if (tokens[i] == PIPE)
		{
			if (i == 0 || (i > 0 && tokens[i -1] == PIPE) || tokens[i +1] == PIPE)
			{
				ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
				*err_code = 258;
				return (1);
			}
			if (tokens[i +1] == 0)
			{
				ft_putendl_fd("empty pipe", 2);
				*err_code = 258;
				return (1);
			}
		}
		else if (tokens[i] == IN_FD)
		{
			if (tokens[i +1] != CMD)
				return (syntax_err(array, err_code, i));
		}
		else if (tokens[i] == IN_HD)
		{
			if (tokens[i +1] != CMD)
				return (syntax_err(array, err_code, i));
		}
		else if (tokens[i] == OUT)
		{
			if (tokens[i +1] != CMD)
				return (syntax_err(array, err_code, i));
		}
		else if (tokens[i] == OUT_AP)
		{
			if (tokens[i +1] != CMD)
				return (syntax_err(array, err_code, i));
		}
		else if (tokens[i] == NOT_MS)
		{
			ft_putstr_fd("minishell: syntax error: non implemented operator: ", 2);
			ft_putendl_fd(array[i], 2);
			*err_code = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

int	parsing(char **line_read, t_pipe ***pipe, t_utils *utils)
{
	char	**array;
	int		*tokens;

	array = NULL;
	if (lexer(line_read, utils) == 1)
		return (1);
	if (*line_read == NULL || (*line_read)[0] == '\0')
		return (1);
	array = ms_split(*line_read);
	free(*line_read);
	*line_read = NULL;
	if (!array)
		return (1); // or  something else if malloc failed
	init_tokenarr(&tokens, array);
	if (syntax_check(tokens, &utils->err_code, array) == 1)
		utils->syntax_err = TRUE;
	pipeline_init(array, pipe);
	if (parser(array, pipe, &utils->err_code) == 1)
		return (1); // free all first
	here_doc(pipe);
	ft_arrfree(array);
	if (utils->syntax_err == TRUE)
		return (1); //free what is needed first
	return (0);
}

void	here_doc(t_pipe ***pipe)
{
	int	i;
	int	x;

	x = 0;
	while ((*pipe)[x])
	{
		i = 0;
		while ((*pipe)[x]->args[i])
		{
			if ((*pipe)[x]->tokens[i] == SKIP_HD)
			{
				here_doc_open((*pipe)[x]->args[i], (*pipe)[x]);
				close ((*pipe)[x]->hd_fd[0]);
			}
			else if ((*pipe)[x]->tokens[i] == IN_HD)
				here_doc_open((*pipe)[x]->args[i], (*pipe)[x]);
			i++;
		}
		x++;
	}
}


void	here_doc_open(char *eof, t_pipe *_pipe)
{
	char	*buff;

	pipe(_pipe->hd_fd);
	while (1)
	{
		buff = readline("> ");
		if (ft_strncmp(eof, buff, -1) == 0)
			break ;
		ft_putendl_fd(buff, _pipe->hd_fd[1]);
		free (buff);
	}
	close(_pipe->hd_fd[1]);
	free(buff);
}

