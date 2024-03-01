/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:45:09 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/16 18:06:50 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	char	*sub;
// 	size_t	sub_len;

// 	if (!s)
// 		return (NULL);
// 	sub_len = ft_strlen(s);
// 	if (sub_len < start)
// 	{
// 		sub = malloc(1);
// 		if (!sub)
// 			return (NULL);
// 		sub[0] = '\0';
// 		return (sub);
// 	}
// 	sub_len = sub_len - start;
// 	if (sub_len > len)
// 		sub_len = len;
// 	sub = malloc((sub_len + 1) * sizeof(char));
// 	if (!sub)
// 		return (NULL);
// 	ft_strlcpy(sub, (char *)s + start, sub_len + 1);
// 	return (sub);
// }

// size_t	ft_strlen(const char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 		i ++;
// 	return (i);
// }

// size_t	ft_strlcpy(char *dst, char *src, size_t dstsize)
// {
// 	size_t	i;

// 	i = 0;
// 	if (dstsize)
// 	{
// 		while (i < dstsize - 1 && src[i] != '\0')
// 		{
// 			dst[i] = src[i];
// 			i ++;
// 		}
// 		dst [i] = '\0';
// 	}
// 	while (src[i] != '\0')
// 		i ++;
// 	return (i);
// }

void	*free_str(char *str)
{
	if (str)
		free (str);
	return (NULL);
}

size_t	find_line(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] != '\n' && str[i] != '\0')
		i ++;
	if (str[i] == '\n')
		return (i + 1);
	else
		return (0);
}
// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char	*newstr;

// 	if (s1 && s2)
// 	{
// 		newstr = malloc(((ft_strlen(s1) + ft_strlen(s2)) + 1) * sizeof(char));
// 		if (newstr == NULL)
// 			return (NULL);
// 		ft_strcpy(newstr, (char *)s1, (char *)s2);
// 		return (newstr);
// 	}
// 	return (NULL);
// }

// void	ft_strcpy(char *dest, char *src, char*src2)
// {
// 	while (*src != '\0')
// 	{
// 		*dest = *src;
// 		dest ++;
// 		src ++;
// 	}
// 	while (*src2 != '\0')
// 	{
// 		*dest = *src2;
// 		dest ++;
// 		src2 ++;
// 	}
// 	*dest = '\0';
// }
