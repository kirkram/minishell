/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:10:59 by clundber          #+#    #+#             */
/*   Updated: 2024/02/07 15:24:06 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_free(char **array, int iarr)

{
	if (iarr > 0)
		iarr--;
	while (iarr >= 0)
	{
		free(array[iarr]);
		iarr--;
	}
	free (array);
}

static char	*ft_arrayadd(char const *s, int start, int len)

{
	char	*str;
	int		i;

	i = 0;
	str = (char *) malloc(sizeof(char) * (len +1));
	if (!str)
		return (0);
	while (len > 0)
	{
		str[i] = s[start];
		i++;
		start++;
		len--;
	}
	str[i] = '\0';
	return (str);
}

static int	ft_strcount(char const *s, char c)

{
	int	stringcount;
	int	i;

	i = 0;
	stringcount = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
		{
			stringcount++;
			while ((s[i] != c && s[i] != '\0'))
				i++;
		}
	}
	return (stringcount);
}

static int	ft_arraymaker(char **array, int iarr, char const *s, char c)

{
	size_t	i;
	size_t	start;

	i = 0;
	while (i < ft_strlen(s))
	{
		if (s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
		{
			start = i;
			while (s[i] != c && s[i] != '\0')
				i++;
			array[iarr] = ft_arrayadd(s, start, (i - start));
			if (!array[iarr])
			{
				ft_free(array, iarr);
				return (0);
			}
			iarr++;
		}
	}
	array[iarr] = (void *)0;
	return (1);
}

char	**ft_split(char const *s, char c)

{
	char	**array;
	int		iarr;

	iarr = 0;
	if (!s)
		return (0);
	array = malloc(sizeof(char *) * (ft_strcount(s, c) + 1));
	if (!array)
		return (0);
	if (ft_arraymaker(array, iarr, s, c) == 0)
		return (0);
	return (array);
}
