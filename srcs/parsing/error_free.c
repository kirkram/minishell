/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 13:00:58 by clundber          #+#    #+#             */
/*   Updated: 2024/04/02 14:21:46 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	lex_merror(t_utils *utils, char **str)
{
	if ((*str))
		free((*str));
	ft_arrfree(utils->envp);
	ft_arrfree(utils->export);
	if (utils)
		free (utils);
	ft_putendl_fd("malloc error", 2);
	exit(1);
}
