/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:04:39 by klukiano          #+#    #+#             */
/*   Updated: 2024/03/08 14:05:19 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ppx_split(char const *str, char c)
{
	char	**array;
	int		i;

	i = 0;
	if (str)
	{
		array = malloc((countstrings((char *)str, c) + 1) * sizeof(char *));
		if (array == NULL)
			return (NULL);
		if (countstrings((char *)str, c) == 0)
		{
			free (array);
			return (NULL);
		}
		else
			splitter((char *)str, c, array, i);
		return (array);
	}
	return (NULL);
}

char	**splitter(char *str, char c, char **array, int i)
{
	int		j;
	int		x;
	char	stop_char;

	x = 0;
	while (str[x] != '\0')
	{
		j = 0;
		while (str[x] == c)
			x ++;
		array[i] = malloc(splitlen(str + x, c) + 1 * sizeof(char));
		if (array[i] == NULL)
			return (free_reverse(i, array));
		stop_char = skip_chars(&x, str, stop_char, c);
		while (str[x] != stop_char && str[x] != '\0')
		{
			array[i][j] = str[x];
			j ++;
			x ++;
		}
		array[i][j] = '\0';
		skip_esc_and_c(stop_char, &x, str, c);
		i ++;
	}
	return (return_and_nullterm(&array, i));
}

int	splitlen(char *str, char c)
{
	int		i;
	char	stop_char;

	i = 0;
	if (str[i] == '\'' || str[i] == '\"')
	{
		stop_char = *str;
		str ++;
		while (str[i] != stop_char && str[i] != '\0')
			i ++;
		return (i);
	}
	while (str[i] != c && str[i] != '\0')
		i ++;
	return (i);
}

int	countstrings(char *str, char c)
{
	int		count;
	char	stop_char;

	count = 0;
	while (*str != '\0')
	{
		while (*str == c && *str != '\0' && *str != '\'' && *str != '\"')
			str ++;
		if (*str != '\0')
			count ++;
		if (*str == '\'' || *str == '\"')
		{
			stop_char = *str;
			str ++;
			while (*str != stop_char && *str)
				str ++;
			if (*str)
				str ++;
		}
		else
			while (*str != c && *str != '\0')
				str ++;
	}
	return (count);
}
