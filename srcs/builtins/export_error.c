/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 12:03:03 by clundber          #+#    #+#             */
/*   Updated: 2024/04/22 10:42:25 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	print_exp_error(char *arg, int prog)
{
	if (prog == 1)
		ft_putstr_fd("MINISHELL: export: `", 2);
	else
		ft_putstr_fd("MINISHELL: unset: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("\': not a valid identifier", 2);
	return (1);
}

int	export_error(char *arg, int prog)
{
	int	x;

	x = 0;
	while (arg[x] && (arg[x] == '\'' || arg[x] == '\"'))
		x++;
	if ((arg[x] >= '0' && arg[x] <= '9') || arg[x] == '=')
		return (print_exp_error(arg, prog));
	while (arg[x] && arg[x] != '=')
	{
		if (!((arg[x] >= '0' && arg[x] <= '9') || (arg[x] >= 'a'
					&& arg[x] <= 'z') || (arg[x] >= 'A'
					&& arg[x] <= 'Z') || (arg[x] == '_')))
			return (print_exp_error(arg, prog));
		x++;
	}
	return (0);
}
