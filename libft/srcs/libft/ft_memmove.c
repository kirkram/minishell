/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 12:04:54 by clundber          #+#    #+#             */
/*   Updated: 2024/02/07 15:23:58 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)

{
	char		*dstptr;
	const char	*srcptr;

	srcptr = src;
	dstptr = dst;
	if (!dst && !src)
		return (0);
	if (dstptr > srcptr)
	{
		while (len > 0)
		{
			dstptr[len -1] = srcptr[len -1];
			len--;
		}
	}
	while (len > 0)
	{
		*dstptr = *srcptr;
		dstptr++;
		srcptr++;
		len--;
	}
	return (dst);
}
