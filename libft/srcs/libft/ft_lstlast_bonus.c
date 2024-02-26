/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:34:35 by clundber          #+#    #+#             */
/*   Updated: 2024/02/07 15:23:51 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)

{
	t_list	*ptr;

	ptr = NULL;
	if (!lst)
		return (0);
	ptr = lst;
	while (ptr->next)
		ptr = ptr->next;
	return (ptr);
}
