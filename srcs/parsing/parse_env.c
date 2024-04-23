/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 18:44:51 by clundber          #+#    #+#             */
/*   Updated: 2024/04/23 14:52:37 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	env_loop(t_ms *ms, int *start, int *i, char **str)
{
	{
		if (!ms->temp)
			ms->temp = ft_substr((*str), (*start), (*i));
		else
			combine_str(&ms->temp,
				ft_substr((*str), (*start), (*i) - (*start)));
		malloc_check(&ms->temp, ms);
		(*i)++;
		(*start) = (*i);
		if ((*str)[(*i)] <= '0' || (*str)[(*i)] >= '9')
		{
			while ((*str)[(*i)] && (((*str)[(*i)] >= '0' && (*str)[(*i)] <= '9')
					|| ((*str)[(*i)] >= 'a' && (*str)[(*i)] <= 'z') \
					|| ((*str)[(*i)] >= 'A' && (*str)[(*i)] <= 'Z') \
					|| (*str)[(*i)] == '_' \
					|| (*str)[(*i)] == '?') && (*str)[(*i) - 1] != '?')
				(*i)++;
		}
		else
			(*i)++;
	}
}

void	env_util(t_ms *ms, int start, int i, char **str)
{
	combine_str(&ms->temp, ft_substr((*str), start, i - start));
	malloc_check(&ms->temp, ms);
	ft_nullfree (str);
	(*str) = ft_strdup(ms->temp);
	malloc_check(str, ms);
	ft_nullfree(&ms->temp);
}

static int	hd_c(char **str, int *i)
{
	int	x;

	x = (*i);
	x--;
	while (x >= 0)
	{
		while (x > 0 && (((*str)[x] >= 9 \
			&& (*str)[x] <= 13) || (*str)[x] == 32))
			x--;
		if (x > 0 && (*str)[x] == '<')
			x--;
		else
			return (0);
		if (x > 0 && (*str)[x] == '<' && (*str)[x -1] != '<')
			return (1);
		else if (x == 0 && (*str)[x] == '<')
			return (1);
		else
			return (0);
	}
	return (0);
}

void	env_variable(t_ms *ms, bool quote, bool dquote, char **str)
{
	int		start;
	int		i;

	i = 0;
	start = 0;
	while ((*str) && (*str)[i])
	{
		quote_status2(&quote, &dquote, (*str)[i]);
		if ((*str)[i] == '$' && quote == false && (*str)[i +1] && (((*str)[i +1]
			>= '0' && (*str)[i +1] <= '9') || ((*str)[i +1] >= 'a' && (*str) \
			[i +1] <= 'z') || ((*str)[i +1] >= 'A' && (*str)[i +1] <= 'Z') || \
			(*str)[i +1] == '_' || (*str)[i +1] == '?') && hd_c(str, &i) == 0)
		{
			env_loop(ms, &start, &i, str);
			ms->temp2 = ft_substr((*str), start, i - start);
			malloc_check(&ms->temp2, ms);
			combine_str(&ms->temp, get_variable(ms));
			malloc_check(&ms->temp, ms);
			start = i;
		}
		else
			i++;
	}
	if (ms->temp)
		env_util(ms, start, i, str);
}
