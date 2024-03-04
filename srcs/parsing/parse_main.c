/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 15:50:37 by clundber          #+#    #+#             */
/*   Updated: 2024/03/04 17:02:48 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	error_func(char *str, int err)

{
	ft_putendl_fd(str, 2);
	g_signal = err;
}

// int	main(int argc, char *argv[], char *envp[])

// {
// 	//t_pipe		**pipe;
// 	int			i;
// 	int			x;
// 	//pipe = malloc(sizeof (t_pline));
// 	i = 0;
// 	if (argc < 2 || !argv[1][0])
// 		return (0);
// 	lexer(argv[1], envp);
// }
