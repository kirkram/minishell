/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 18:44:51 by clundber          #+#    #+#             */
/*   Updated: 2024/04/18 11:12:10 by clundber         ###   ########.fr       */
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

void	env_variable(t_ms *ms, bool quote, bool dquote, char **str)
{
	int		start;
	int		i;

	i = 0;
	start = 0;
	while ((*str) && (*str)[i])
	{
		quote_status2(&quote, &dquote, (*str)[i]);
		if ((*str)[i] == '$' && quote == false && (*str)[i +1]
			&& (*str)[i +1] != ' ' && (*str)[i +1] != '$'
			&& (!(dquote == true && ((*str)[i +1] == '\''
			|| (*str)[i +1] == '\"' || (*str)[i +1] == ' '))))
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

char	*get_variable(t_ms *ms)
{
	int		i;
	char	*env_var;

	env_var = NULL;
	i = 0;
	if (env_error(&ms->temp2, ms) == 1)
		return (ft_itoa(ms->utils->err_code));
	while (ms->utils->envp[i])
	{
		if (ft_strnstr(ms->utils->envp[i], ms->temp2, ft_strlen(ms->temp2)))
		{
			env_var = ft_substr(ms->utils->envp[i], ft_strlen(ms->temp2),
					ft_strlen(ms->utils->envp[i]) - ft_strlen(ms->temp2));
			malloc_check(&env_var, ms);
			break ;
		}
		i++;
	}
	if (ms->temp2)
		ft_nullfree(&ms->temp2);
	if (!env_var)
		return (ft_strdup(""));
	return (env_var);
}
