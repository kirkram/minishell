/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_split_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:04:39 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/27 11:18:55 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	skip_chars(int *x, char *str, char stop_char, char c)
{
	stop_char = c;
	if (str[*x] == '\'' || str[*x] == '\"')
	{
		stop_char = str[*x];
	}
	if (str[*x] == '\'' || str[*x] == '\"')
		(*x)++;
	return (stop_char);
}

char	**return_and_nullterm(char ***array, int i)
{
	(*array)[i] = NULL;
	return (*array);
}

void	skip_esc_and_c(char stop_char, int *x, char *str, char c)
{
	if (str[*x] && (stop_char == '\'' || stop_char == '\"'))
		(*x)++;
	while (str[*x] == c && str[*x] != '\0')
		(*x)++;
}

char	**free_reverse(int i, char **array)
{
	while (i >= 0)
	{
		free (array[i]);
		i --;
	}
	free (array);
	return (NULL);
}
