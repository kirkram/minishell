/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 18:44:51 by clundber          #+#    #+#             */
/*   Updated: 2024/03/28 17:26:33 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	env_loop(char **str, char **new_str, int *start, int *i)
{
	{
		if (!(*new_str))
			(*new_str) = ft_substr((*str), (*start), (*i));
		else
			combine_str(new_str, ft_substr((*str), (*start), (*i) - (*start)));
		if (!(*new_str))
			malloc_error (1);
		(*i)++;
		(*start) = (*i);
		while ((*str)[(*i)] && (*str)[(*i)] != ' ' && (*str)[(*i)] != '\0'
			&& (*str)[(*i)] != '$' && (*str)[(*i)] != '\'' && (*str)[(*i)]
			!= '\"' && (*str)[(*i) - 1] != '?' && (*str)[(*i)] != '/')
			(*i)++;
	}
}

void	env_loop2(char **temp, char **new_str, t_utils *utils)
{
	if (!(*temp))
		malloc_error (1);
	combine_str(new_str, get_variable((*temp),
			utils->envp, utils->err_code));
	if (!(*new_str))
		malloc_error (1);
}

void	env_util(char **new_str, char **str, int start, int i)
{
	combine_str(new_str, ft_substr((*str), start, i - start));
	if (!(*new_str))
		malloc_error (1);
	(*str) = (*new_str);
}

void	env_variable(char **str, t_utils *utils, bool quote, bool dquote)
{
	int		start;
	char	*new_str;
	char	*temp;
	int		i;

	i = 0;
	temp = NULL;
	new_str = NULL;
	start = 0;
	while ((*str) && (*str)[i])
	{
		quote_status2(&quote, &dquote, (*str)[i]);
		if ((*str)[i] == '$' && quote == false && (*str)[i +1]
			&& (*str)[i +1] != ' ' && (*str)[i +1] != '$')
		{
			env_loop(str, &new_str, &start, &i);
			temp = ft_substr((*str), start, i - start);
			env_loop2(&temp, &new_str, utils);
			start = i;
		}
		else
			i++;
	}
	if (new_str)
		env_util(&new_str, str, start, i);
}

char	*get_variable(char *temp, char **envp, int err_code)
{
	int		i;
	char	*env_var;

	env_var = NULL;
	i = 0;
	if (env_error(&temp) == 1)
		return (ft_itoa(err_code));
	while (envp[i])
	{
		if (ft_strnstr(envp[i], temp, ft_strlen(temp)))
		{
			env_var = ft_substr(envp[i], ft_strlen(temp),
					ft_strlen(envp[i]) - ft_strlen(temp));
			if (!env_var)
				malloc_error(1);
			break ;
		}
		i++;
	}
	if (temp)
		free (temp);
	if (!env_var)
		return (ft_strdup(""));
	return (env_var);
}
