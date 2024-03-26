/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 14:21:44 by clundber          #+#    #+#             */
/*   Updated: 2024/03/26 16:36:34 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_variable(char *temp, char **envp, int err_code)
{
	int		i;
	char	*env_var;

	env_var = NULL;
	i = 0;
	temp = ft_strjoin(temp, "=");
	if (!temp)
		malloc_error (1);
	if (ft_strnstr("?=", temp, ft_strlen(temp)))
		return (ft_itoa(err_code));
	while (envp[i])
	{
		if (ft_strnstr(envp[i], temp, ft_strlen(temp)))
		{
			env_var = ft_substr(envp[i], ft_strlen(temp), ft_strlen(envp[i]) - ft_strlen(temp));
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

void	combine_str(char **new_str, char *temp)
{
	char	*ptr;

	ptr = NULL;
	ptr = *new_str;
	if ((*new_str) && temp)
		(*new_str) = ft_strjoin((*new_str), temp);
	else
		(*new_str) = NULL;
	if (!(*new_str))
		malloc_error(1);
	free (temp);
	free (ptr);
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
		if ((*str)[i] == '$' && quote == false && (*str)[i +1] && (*str)[i +1] != ' ' && (*str)[i +1] != '$')
		{
			if (!new_str)
				new_str = ft_substr((*str), start, i);
			else
				combine_str(&new_str, ft_substr((*str), start, i - start));
			if (!new_str)
				malloc_error (1);
			i++;
			start = i;
			while ((*str)[i] && (*str)[i] != ' ' && (*str)[i] != '\0' && (*str)[i] != '$' &&(*str)[i] != '\'' && (*str)[i] != '\"' && (*str)[i -1] != '?')
				i++;
			temp = ft_substr((*str), start, i - start);
			if (!temp)
				malloc_error (1);
			combine_str(&new_str, get_variable(temp, utils->envp, utils->err_code));
			if (!new_str)
				malloc_error (1);
			start = i;
		}
		else
			i++;
	}
	if (new_str)
	{
		combine_str(&new_str, ft_substr((*str), start, i - start));
		if (!new_str)
			malloc_error (1);
		(*str) = new_str;
	}
}

void	remove_space(char **str, int i)
{
	int		end;
	char	*temp;

	temp = NULL;
	while ((*str)[i])
		i++;
	if (i > 0)
		i--;
	end = i;
	while ((*str)[i] == ' ')
		i--;
	if (i != end && i > 0)
	{
		temp = ft_substr((*str), 0, i +1);
		if (!temp)
			malloc_error(1);
		free (*str);
		(*str) = temp;
	}
}

int	check_quote(char **str, bool quote, bool dquote, int *err_code)
{
	int	i;

	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '\"' && quote == false)
			quote_status(&dquote);
		if ((*str)[i] == '\'' && dquote == false)
			quote_status(&quote);
		i++;
	}
	if (quote == true || dquote == true)
	{
		ft_putendl_fd("error: unclosed quotes detected", 2);
		*err_code = 1;
		return (1);
	}
	return (0);
}

int	lexer(char **str, t_utils *utils)
{
	bool	quote;
	bool	dquote;
	int		i;

	i = 0;
	quote = false;
	dquote = false;
	if (check_quote(str, quote, dquote, &utils->err_code) == 1)
		return (1);
	remove_space(str, i);
	while ((*str)[i])
	{
		if ((*str)[i] == '$')
		{
			env_variable(str, utils, quote, dquote);
			break ;
		}
		i++;
	}
	*str = separator(*(str), quote, dquote);
	return (0);
}
