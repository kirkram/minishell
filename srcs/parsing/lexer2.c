/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 21:46:03 by clundber          #+#    #+#             */
/*   Updated: 2024/03/20 15:14:53 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// at the moment, assigns too much memory
char	*separator(char *str, bool quote, bool dquote)
{
	int		i;
	int		x;
	char	*temp;
	int		count;

	count = 0;
	i = 0;
	x = 0;
	while (str && str[i])
	{
		if (str[i] == '\"' && quote == false)
			quote_status(&dquote);
		if (str[i] == '\'' && dquote == false)
			quote_status(&quote);
		if (quote == false && dquote == false)
		{
			if (str[i] == '>' || str[i] == '|' || str[i] == '<')// && (i == 0 || (i > 0 && str[i -1] != str[i])))
				x++;
		}
		i++;
	}
	temp = malloc(sizeof(char) * (i + (x *2) + 1));
	if (!temp)
		malloc_error(1);
	i = 0;
	x = 0;
	quote = false;
	dquote = false;
	while (str && str[i])
	{
		if (str[i] == '\'')
			quote_status(&quote);
		else if (str[i] == '\"')
			quote_status(&dquote);
		if ((str[i] == '>' || str[i] == '|' || str[i] == '<') && (quote == false && dquote == false))
		{
			temp[x] = ' ';
			x++;
			while (str[i] && count <= 1 && str[i] == str[i - count])
			{
				count++;
				temp[x] = str[i];
				i++;
				x++;
			}
			count = 0;
			if (str[i] == '<' && i >= 2 && str[i -1] == '<' && str[i -2 == '<'])
			{
				temp[x] = str[i];
				x++;
				i++;
			}
			temp[x] = ' ';
			x++;
		}
		else
		{
			temp[x] = str[i];
			x++;
			i++;
		}
	}
	temp[x] = '\0';
	return (temp);
}







