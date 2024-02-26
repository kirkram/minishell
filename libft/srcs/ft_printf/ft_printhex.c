/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printhex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 11:18:56 by clundber          #+#    #+#             */
/*   Updated: 2024/02/07 15:23:22 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printhex(unsigned long nbr, int caps)

{
	int		count;
	char	*hex;
	char	output;
	int		temp;

	temp = 0;
	count = 0;
	if (caps == 0)
		hex = "0123456789abcdef";
	else
		hex = "0123456789ABCDEF";
	output = hex[nbr % 16];
	if (nbr / 16 != 0)
		count += ft_printhex((nbr / 16), caps);
	if (temp > count)
		return (-1);
	temp = count;
	count += ft_putchar_return(output);
	return (count);
}
