/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:44:03 by clundber          #+#    #+#             */
/*   Updated: 2024/02/26 15:24:01 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	initiate_node(t_cmd *cmd_node)

{


	
}

void	lexer(t_cmd *head, char *argv)

{



}

int	main(int argc, char *argv[])

{
	t_cmd	*cmd_head;

	cmd_head = NULL;
	lexer(cmd_head, argv[1]);
}
