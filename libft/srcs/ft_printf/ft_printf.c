/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:59:15 by clundber          #+#    #+#             */
/*   Updated: 2024/02/19 15:49:13 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdarg.h>
#include "ft_printf.h"

static int	ft_print_special(const char s, va_list ap)

{
	int	count;

	count = 0;
	if (s == 'c')
		count += ft_putchar_return(va_arg(ap, int));
	else if (s == 's')
		count += ft_putstr_return(va_arg(ap, char *));
	else if (s == 'd' || s == 'i')
		count += ft_putnbr_return(va_arg(ap, int));
	else if (s == 'u')
		count += ft_putnbr_u_return(va_arg(ap, unsigned int));
	else if (s == 'x')
		count += ft_printhex(va_arg(ap, unsigned int), 0);
	else if (s == 'X')
		count += ft_printhex(va_arg(ap, unsigned int), 1);
	else if (s == 'p')
	{
		count += write(1, "0x", 2);
		if (count == -1)
			return (-1);
		count += ft_printhex(va_arg(ap, long long), 0);
	}
	else
		count += write(1, &s, 1);
	return (count);
}

int	ft_printf(const char *str, ...)

{
	va_list	ap;
	int		counter;
	int		error;

	error = 0;
	counter = 0;
	va_start(ap, str);
	while (*str)
	{
		if (*str == '%' && str++)
			counter += ft_print_special(*str, ap);
		else
			counter += write(1, str, 1);
		if (error > counter)
		{
			va_end(ap);
			return (-1);
		}
		error = counter;
		str++;
	}
	va_end(ap);
	return (counter);
}
