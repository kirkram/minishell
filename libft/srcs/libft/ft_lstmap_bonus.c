/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 10:55:30 by clundber          #+#    #+#             */
/*   Updated: 2024/02/07 15:23:52 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))

{
	t_list	*new_list;
	t_list	*temp;
	t_list	*content;

	new_list = NULL;
	while (lst)
	{
		content = f(lst->content);
		temp = ft_lstnew(content);
		if (!temp)
		{
			if (content != NULL && content != lst->content)
				(*del)(content);
			ft_lstclear(&new_list, del);
			return (0);
		}
		lst = lst->next;
		ft_lstadd_back(&new_list, temp);
	}
	return (new_list);
}
