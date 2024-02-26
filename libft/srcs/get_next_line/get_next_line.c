/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:47:43 by clundber          #+#    #+#             */
/*   Updated: 2024/02/08 14:01:47 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"
#include "libft.h"

int	gnlinecheck(char *str)

{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == '\n')
			return (1);
		str++;
	}
	return (0);
}

char	*ft_reader(char *temp, int fd)
{
	char	*buffer;
	char	*ptr;
	int		rd;

	ptr = temp;
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (ft_gnlfree(&temp));
	rd = read(fd, buffer, BUFFER_SIZE);
	if (rd <= 0)
		return (rd_check(buffer, temp, rd));
	buffer[rd] = '\0';
	if (!temp)
		temp = ft_substr(buffer, 0, rd);
	else
	{
		temp = ft_strjoin(ptr, buffer);
		free (ptr);
	}
	free (buffer);
	if (!temp)
		return (0);
	if (gnlinecheck(temp) == 0)
		temp = ft_reader(temp, fd);
	return (temp);
}

char	*ft_rowmaker(char *temp, char *row)

{
	int		i;

	i = 0;
	if (!temp)
		return (0);
	while (temp[i])
	{
		if (temp[i] == '\n')
		{
			row = ft_substr(temp, 0, i +1);
			if (!row)
				return (0);
			return (row);
		}
		else if (temp[i +1] == '\0')
		{
			row = ft_strdup(temp);
			if (!row)
				return (0);
			return (row);
		}
		i++;
	}
	return (0);
}

char	*ft_gnlfree(char **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
	return (0);
}

char	*get_next_line(int fd)

{
	char		*row;
	static char	*temp[256];
	char		*ptr;

	row = NULL;
	ptr = NULL;
	if (fd < 0 || fd > 256 || BUFFER_SIZE <= 0)
		return (0);
	if (gnlinecheck(temp[fd]) == 0)
		temp[fd] = ft_reader(temp[fd], fd);
	if (!temp[fd])
		return (0);
	row = ft_rowmaker(temp[fd], row);
	if (!row)
		return (ft_gnlfree(&temp[fd]));
	if (gnlinecheck(row) == 1 && (ft_len(row) < ft_len(temp[fd])))
	{
		ptr = temp[fd];
		temp[fd] = ft_substr(ptr, ft_len(row), ft_len(temp[fd]) - ft_len(row));
		ft_gnlfree(&ptr);
		return (row);
	}
	ft_gnlfree(&temp[fd]);
	return (row);
}
