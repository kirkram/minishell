/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 09:56:37 by clundber          #+#    #+#             */
/*   Updated: 2024/02/07 15:23:55 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)

{
	int		counter;
	t_list	*ptr;

	ptr = NULL;
	counter = 0;
	if (!lst)
		return (0);
	ptr = lst;
	while (ptr)
	{
		counter++;
		ptr = ptr->next;
	}
	return (counter);
}
