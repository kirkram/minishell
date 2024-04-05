/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 11:18:05 by clundber          #+#    #+#             */
/*   Updated: 2024/04/04 12:14:47 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ms_split(char *str)
{
	char	**array;
	bool	quote;
	bool	dquote;

	quote = false;
	dquote = false;
	array = NULL;
	if (!str)
		return (0);
	if (str_count(str) == 0)
		return (0);
	if (str)
	{
		if (str_count(str) == 0)
			return (0);
		array = malloc((str_count(str) + 1) * sizeof(char *));
		if (!array)
			return (0);
		ms_splitter(str, array, quote, dquote);
		return (array);
	}
	return (0);
}

char	*remove_quote(char *str, int i)
{
	int		x;
	char	*temp;
	bool	quote;
	bool	dquote;

	i = 0;
	x = 0;
	quote = false;
	dquote = false;
	temp = malloc((ft_strlen(str) - quote_count(str) +1) * sizeof(char));
	if (!temp || !str)
		return (NULL);
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
	free (str);
	return (temp);
}

int	quote_count(char *str)
{
	int		i;
	int		count;
	bool	quote;
	bool	dquote;

	i = 0;
	count = 0;
	quote = false;
	dquote = false;
	while (str[i])
	{
		if (str[i] == '\'' && dquote == false)
		{
			quote_status(&quote);
			count++;
		}
		if (str[i] == '\"' && quote == false)
		{
			quote_status(&dquote);
			count++;
		}
		i++;
	}
	return (count);
}

char	**ms_splitter(char *str, char **array, bool quote, bool dquote)
{
	int		i;
	int		x;
	int		start;

	i = 0;
	x = 0;
	start = 0;
	while (str[x])
	{
		while (str[x] == ' ')
			x++;
		start = x;
		while (str[x] && (str[x] != ' ' || quote == true || dquote == true))
		{
			quote_status2(&quote, &dquote, str[x]);
			x++;
		}
		array[i] = ft_substr(str, start, (x - start));
		if (!array[i])
			return (free_reverse(i -1, array));
		i++;
	}
	array[i] = NULL;
	return (array);
}

int	str_count(char *str)
{
	int		count;
	bool	quote;
	bool	dquote;
	int		x;

	x = 0;
	quote = false;
	dquote = false;
	count = 1;
	while (str[x])
	{
		while (str[x] == ' ' && dquote == false && quote == false)
			x++;
		if (str[x])
			count++;
		while (str[x] && (str[x] != ' ' || quote == true || dquote == true))
		{
			if (str[x] == '\'' && dquote == false)
				quote_status(&quote);
			else if (str[x] == '\"' && quote == false)
				quote_status(&dquote);
			x++;
		}
	}
	return (count);
}
