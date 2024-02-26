/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 17:03:04 by clundber          #+#    #+#             */
/*   Updated: 2024/02/07 15:23:46 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strmaker(char *str, long int integer, int len)

{
	if (integer < 0)
	{
		str[0] = '-';
		integer *= -1;
	}
	str[len] = '\0';
	len--;
	while (integer > 0)
	{
		str[len] = (integer % 10 + 48);
		integer /= 10;
		len--;
	}
	return (str);
}

char	*ft_itoa(int n)

{
	char		*str;
	long int	integer;
	int			len;

	integer = n;
	len = 0;
	while (integer != 0)
	{
		integer /= 10;
		len++;
	}
	if (n <= 0)
		len++;
	integer = n;
	str = (char *) malloc((len +1) * sizeof (char));
	if (!str)
		return (0);
	if (integer == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return (str);
	}
	str = ft_strmaker(str, integer, len);
	return (str);
}
