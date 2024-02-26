/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 18:18:03 by clundber          #+#    #+#             */
/*   Updated: 2024/02/07 15:23:33 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	ft_overflow(int neg, const char *str)

{
	if (str[neg] == '-')
		return (0);
	else
		return (-1);
}

int	ft_atoi(const char *str)

{
	long int	num;
	int			i;
	int			neg;
	long int	buf;

	i = 0;
	num = 0;
	buf = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	neg = i;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= 48 && str[i] <= 57)
	{
		num = (num * 10) + (str[i] - 48);
		i++;
		if (buf > num)
			return (ft_overflow(neg, str));
		buf = num;
	}
	if (str[neg] == '-')
		num *= -1;
	return (num);
}
