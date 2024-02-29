/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:58:13 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/29 10:28:48 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->line = NULL;
	node->cells = NULL;
	node->int_array = NULL;
	node->color_array = NULL;
	node->is_colored = 0;
	node->width = 0;
	node->height = 0;
	node->amount = 0;
	node->content = content;
	node->next = NULL;
	return (node);
}
