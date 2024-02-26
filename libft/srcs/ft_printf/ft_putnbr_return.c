/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_return.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 11:30:58 by clundber          #+#    #+#             */
/*   Updated: 2024/02/07 15:23:23 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_putnbr_return(int n)

{
	char			output;
	unsigned int	nb;
	int				count;
	int				temp;

	temp = 0;
	count = 0;
	if (n < 0)
	{
		count += write (1, "-", 1);
		n = -n;
	}
	if (count == -1)
		return (-1);
	nb = n;
	output = nb % 10 + 48;
	if (nb / 10 != 0)
		count += ft_putnbr_return(nb / 10);
	if (temp > count)
		return (-1);
	temp = count;
	count += write(1, &output, 1);
	return (count);
}
