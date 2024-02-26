/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 14:56:26 by clundber          #+#    #+#             */
/*   Updated: 2024/02/07 15:23:56 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*stack;
	unsigned char	needle;

	needle = (unsigned char)c;
	stack = (unsigned char *)s;
	while (n > 0)
	{
		if (*stack == needle)
			return ((void *) stack);
		stack++;
		n--;
	}
	return (0);
}
