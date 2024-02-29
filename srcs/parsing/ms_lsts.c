/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lsts.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:06:39 by clundber          #+#    #+#             */
/*   Updated: 2024/02/29 13:04:59 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "parsing.h"

t_bigcmd	*ms_lstlast(t_bigcmd *lst)

{
	t_bigcmd	*ptr;

	ptr = NULL;
	if (!lst)
		return (0);
	ptr = lst;
	while (ptr->next)
		ptr = ptr->next;
	return (ptr);
}

t_bigcmd	*ms_lstnew(char **cmds)

{
	t_bigcmd	*new_node;

	new_node = (t_bigcmd *) malloc(sizeof(t_bigcmd));
	if (!new_node)
		return (0);
	new_node->cmds = array_copy(cmds);
	new_node->next = NULL;
	return (new_node);
}

int	ms_lstadd_back(t_bigcmd **lst, t_bigcmd *new)

{
	if (!new)
		return (0);
	if (*lst)
		ms_lstlast(*lst)->next = new;
	else
		*lst = new;
	return (1);
}

int	ms_lstsize(t_bigcmd **lst)

{
	int		counter;
	t_bigcmd	*ptr;

	ptr = NULL;
	counter = 0;
	if (!lst)
		return (0);
	ptr = (*lst);
	while (ptr)
	{
		counter++;
		ptr = ptr->next;
	}
	return (counter);
}

void	lst_clear(t_bigcmd **stack)

{
	t_bigcmd	*ptr;
	t_bigcmd	*ptr2;

	if (*stack)
	{
		ptr = *stack;
		while (ptr->next)
		{
			ptr2 = ptr->next;
			free(ptr);
			ptr = ptr2;
		}
		free (ptr);
	}
}
