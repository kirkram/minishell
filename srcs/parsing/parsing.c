/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:44:03 by clundber          #+#    #+#             */
/*   Updated: 2024/04/09 15:42:24 by clundber         ###   ########.fr       */
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

char	**get_cmd(char **cmds, int start, int end, t_ms *ms)
{
	char	**temp_arr;
	int		i;

	i = 0;
	temp_arr = NULL;
	temp_arr = malloc(sizeof (char *) * ((end - start) + 2));
	if (!temp_arr)
		malloc_check(NULL, ms);
	while (cmds[start] && start <= end)
	{
		temp_arr[i] = ft_strdup(cmds[start]);
		if (!temp_arr[i])
		{
			ft_arrfree(temp_arr);
			malloc_check(NULL, ms);
		}
		i++;
		start++;
	}
	temp_arr[i] = NULL;
	return (temp_arr);
}

void	pre_parse(char **array, t_pipe ***pipe, t_ms *ms)
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
			(*pipe)[x]->args = get_cmd(array, start, i -1, ms);
			x++;
			start = i +1;
		}
		else if (array[i +1] == 0)
		{
			(*pipe)[x]->args = get_cmd(array, start, i, ms);
			break ;
		}
		i++;
	}
}

void	quote_remover(t_pipe *pipe)
{
	int	i;

	i = 0;
	while (pipe->args[i])
	{
		pipe->args[i] = remove_quote(pipe->args[i], i);
		i++;
	}
}

int	parsing(t_ms *ms)//t_pipe ***pipe, t_utils *utils)
{
	char	**array;
	int		*tokens;

	array = NULL;
	ms->utils->syntax_err = FALSE;
	if (lexer(ms) == 1)
		return (1);
	if (ms->line == NULL || ms->line[0] == '\0')
		return (1);
	array = ms_split(ms->line, ms);
	ft_nullfree(&ms->line);
	if (!array)
		return (1);
	init_tokenarr(&tokens, array, ms);
	if (syntax_check(tokens, &ms->utils->err_code, array) == 1)
		ms->utils->syntax_err = TRUE;
	pipeline_init(array, &ms->pipe, ms);
	free (tokens);
	parser(array, &ms->pipe, ms);
	here_doc(&ms->pipe, ms->utils);
	//ft_arrfree(array);
	if (ms->utils->syntax_err == TRUE)
		return (1);
	return (0);
}
