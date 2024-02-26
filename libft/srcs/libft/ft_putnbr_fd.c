/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 10:45:55 by welhox            #+#    #+#             */
/*   Updated: 2024/02/07 15:24:04 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)

{
	char			output;
	unsigned int	nb;

	if (n < 0)
	{
		write (fd, "-", 1);
		n = -n;
	}
	nb = n;
	output = nb % 10 + 48;
	if (nb / 10 != 0)
		ft_putnbr_fd(nb / 10, fd);
	write(fd, &output, 1);
}
