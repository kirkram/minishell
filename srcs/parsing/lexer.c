/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 14:21:44 by clundber          #+#    #+#             */
/*   Updated: 2024/04/03 11:39:12 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	env_error(char **temp)
{
	char	*ptr;

	ptr = *temp;
	(*temp) = ft_strjoin((*temp), "=");
	if (!(*temp))
		malloc_error (1);
	if (ptr)
		free (ptr);
	if (ft_strnstr("?=", (*temp), ft_strlen((*temp))))
	{
		free ((*temp));
		return (1);
	}
	return (0);
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

void	remove_space(char **str, int i, t_utils *utils)
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
			lex_merror(utils, str);
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
	remove_space(str, i, utils);
	while ((*str)[i])
	{
		if ((*str)[i] == '$')
		{
			env_variable(str, utils, quote, dquote);
			break ;
		}
		i++;
	}
	i = 0;
	*str = separator(*(str), quote, dquote, i);
	return (0);
}
