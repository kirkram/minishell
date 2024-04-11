/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:44:03 by clundber          #+#    #+#             */
/*   Updated: 2024/04/10 19:12:20 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

char	*remove_quote(char *str, int i, int x, t_ms *ms)
{
	char	*temp;
	bool	quote;
	bool	dquote;

	i = 0;
	quote = false;
	dquote = false;
	if (!str)
		return (NULL);
	temp = malloc((ft_strlen(str) - quote_count(str) +1) * sizeof(char));
	malloc_check(&temp, ms);
	while (str[i])
	{
		quote_status2(&quote, &dquote, str[i]);
		if (str[i] == '\'' && dquote == false)
			i++;
		else if (str[i] == '\"' && quote == false)
			i++;
		else
			temp[x++] = str[i++];
	}
	temp[x] = '\0';
	ft_nullfree(&str);
	return (temp);
}

void	quote_remover(t_pipe *pipe, t_ms *ms)
{
	int	i;
	int	x;

	x = 0;
	i = 0;
	while (pipe->args[i])
	{
		pipe->args[i] = remove_quote(pipe->args[i], i, x, ms);
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
	here_doc(&ms->pipe, ms->utils, ms);
	if (ms->utils->syntax_err == TRUE)
		return (1);
	return (0);
}
