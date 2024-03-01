/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 15:50:37 by clundber          #+#    #+#             */
/*   Updated: 2024/03/01 13:56:54 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "parsing.h"

int	main(int argc, char *argv[], char *envp[])

{
	//t_pipe		**pipe;
	int			i;
	int			x;
	//pipe = malloc(sizeof (t_pline));
	i = 0;
	if (argc < 2 || !argv[1][0])
		return (0);
	lexer(argv[1], envp);

/* 	while (pipe[i])
	{
		x = 0;
		while (pipe[i]->args[x])
		{
			printf("%s\n", pipe[i]->args[x]);
			x++;
		}
		i++;
	} */




	//tokens = tokenizer(array);
	//listmaker(&cmd_head, array, tokens);
/*	int	x = 0;
 	while (array[x])
	{
		printf("%s    ", array[x]);
		printf("%d\n", tokens[x]);
		x++;
	} */
	//ft_arrfree(array);
	//free (tokens);
}
