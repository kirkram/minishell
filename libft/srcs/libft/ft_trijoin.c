/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trijoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:33:40 by clundber          #+#    #+#             */
/*   Updated: 2024/02/23 10:58:15 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_trijoin(char const *s1, char const *s2, char const *s3)

{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || !s2 || !s3)
		return (0);
	str = malloc ((ft_strlen(s1) + ft_strlen(s2)
				+ ft_strlen(s3) + 1) * sizeof(char));
	if (!str)
		return (0);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
		str[i++] = s2[j++];
	j = 0;
	while (s3[j])
		str[i++] = s3[j++];
	str[i] = '\0';
	return (str);
}
