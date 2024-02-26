/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 17:04:40 by welhox            #+#    #+#             */
/*   Updated: 2024/02/07 15:24:10 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)

{
	int		len;
	char	*dupe;
	int		i;

	i = 0;
	len = 0;
	while (s[len])
		len++;
	dupe = malloc ((len + 1) * sizeof(char));
	if (!dupe)
		return (0);
	while (len >= 0)
	{
		dupe[i] = s[i];
		i++;
		len--;
	}
	return (dupe);
}
