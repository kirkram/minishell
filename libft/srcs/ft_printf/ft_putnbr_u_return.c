/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_u_return.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 11:33:47 by clundber          #+#    #+#             */
/*   Updated: 2024/02/07 15:23:24 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_putnbr_u_return(unsigned int nbr)

{
	char	output;
	int		count;
	int		temp;

	temp = 0;
	count = 0;
	output = nbr % 10 + 48;
	if (nbr / 10 != 0)
		count += ft_putnbr_u_return(nbr / 10);
	if (temp > count)
		return (-1);
	temp = count;
	count += write(1, &output, 1);
	return (count);
}
