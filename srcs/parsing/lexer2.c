/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 21:46:03 by clundber          #+#    #+#             */
/*   Updated: 2024/03/27 18:54:33 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_sep(char **temp, char *str)
{
	bool	quote;
	bool	dquote;
	int		i;
	int		x;

	x = 0;
	i = 0;
	quote = false;
	dquote = false;
	while (str && str[i])
	{
		quote_status2(&quote, &dquote, str[i]);
		if (quote == false && dquote == false)
		{
			if (str[i] == '>' || str[i] == '|' || str[i] == '<')
				x++;
		}
		i++;
	}
	(*temp) = malloc(sizeof(char) * (i +(x * 2) + 1));
	if (!(*temp))
		malloc_error(1);
}

char	*separator(char *str, bool quote, bool dquote, int i)
{
	int		x;
	char	*temp;
	int		count;

	x = 0;
	init_sep(&temp, str);
	while (str && str[i])
	{
		quote_status2(&quote, &dquote, str[i]);
		if ((str[i] == '>' || str[i] == '|' || str[i] == '<')
			&& (quote == false && dquote == false))
		{
			temp[x++] = ' ';
			count = 0;
			while (str[i] && count <= 1 && str[i] == str[i - count++])
				temp[x++] = str[i++];
			if (str[i] == '<' && i >= 2 && str[i -1] == '<' && str[i -2 == '<'])
				temp[x++] = str[i++];
			temp[x++] = ' ';
		}
		else
			temp[x++] = str[i++];
	}
	temp[x] = '\0';
	return (temp);
}
