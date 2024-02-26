/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 16:15:11 by welhox            #+#    #+#             */
/*   Updated: 2024/02/07 15:23:35 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)

{
	void	*ptr;
	size_t	maxsize;

	maxsize = 0 - 1;
	if (nmemb != 0 && size != 0)
	{
		if ((maxsize / nmemb) < size)
			return (0);
	}
	if (nmemb * size > 2147483424)
		return (0);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (0);
	ft_bzero(ptr, (nmemb * size));
	return (ptr);
}
