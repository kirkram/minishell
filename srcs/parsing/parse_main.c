/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 15:50:37 by clundber          #+#    #+#             */
/*   Updated: 2024/03/06 19:39:03 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	main(int argc, char *argv[], char *envp[])

{
 	t_pipe		**pipe;

 	pipe = malloc(sizeof (t_pipe) +1);
 	if (argc < 2 || !argv[1][0])
 		return (0);
 	lexer(argv[1], envp, &pipe);
}
