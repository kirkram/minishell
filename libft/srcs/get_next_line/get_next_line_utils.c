/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:54:17 by clundber          #+#    #+#             */
/*   Updated: 2024/02/08 14:25:16 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>

char	*rd_check(char *buffer, char *temp, int rd)

{
	free (buffer);
	if (rd < 0)
		return (ft_gnlfree(&temp));
	return (temp);
}

size_t	ft_len(const char *s)

{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
