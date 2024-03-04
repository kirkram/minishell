/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:53:32 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/31 12:26:43 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		get_next_line(char **str, int fd);
void	handle_buffer(char	**cache, char **buffer, int bytes_read);
int		handle_cache(char **str, char **cache, int bytes_read, int fd);
int		return_line(char **str, char ***cache, size_t i);
int		handle_read(char **cache, char **buffer, int bytes_read);

int	get_next_line(char **str, int fd)
{
	static char	*cache;
	char		*buffer;
	int			bytes_read;
	int			read_flag;

	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (BUFFER_SIZE < 0 || !buffer)
	{
		if (cache)
			free (cache);
		cache = NULL;
		return (-1);
	}
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	read_flag = handle_read(&cache, &buffer, bytes_read);
	if (read_flag < 1)
		return (read_flag);
	handle_buffer(&cache, &buffer, bytes_read);
	if (handle_cache(str, &cache, bytes_read, fd) == 0)
		return (0);
	else
		return (-1);
}

int	handle_read(char **cache, char **buffer, int bytes_read)
{
	if (bytes_read < 0)
	{
		if (*cache)
			free (*cache);
		*cache = NULL;
		free (*buffer);
		return (-1);
	}
	if (bytes_read == 0)
	{
		if (!*cache)
		{
			free (*buffer);
			return (0);
		}
		if (*cache[0] == '\0')
		{
			free (*cache);
			*cache = NULL;
			free (*buffer);
			return (0);
		}
	}
	return (1);
}

void	handle_buffer(char	**cache, char **buffer, int bytes_read)
{
	char		*tmp_cache;

	(*buffer)[bytes_read] = '\0';
	if (!*cache)
	{
		*cache = ft_substr(*buffer, 0, -1);
		free (*buffer);
	}
	else
	{
		tmp_cache = ft_substr(*cache, 0, -1);
		free (*cache);
		*cache = ft_strjoin(tmp_cache, *buffer);
		free (tmp_cache);
		free (*buffer);
	}
}

int	handle_cache(char **str, char **cache, int bytes_read, int fd)
{
	size_t		i;
	char		*line;

	i = 0;
	if (*cache)
	{
		while ((*cache)[i] != '\n' && (*cache)[i] != '\0')
			i ++;
		if ((*cache)[i] == '\n')
			i += 1;
		else
			i = 0;
	}
	if (i == 0 && bytes_read == 0)
	{
		line = ft_substr(*cache, 0, -1);
		free (*cache);
		*cache = NULL;
		*str = line;
		return (0);
	}
	else if (i == 0)
		return (get_next_line(str, fd));
	else
		return (return_line(str, &cache, i));
}

int	return_line(char **str, char ***cache, size_t i)
{
	char	*line;
	char	*tmp_cache;

	line = ft_substr(**cache, 0, i);
	if (!line)
	{
		free (**cache);
		**cache = NULL;
		return (-1);
	}
	tmp_cache = ft_substr(**cache, i, -1);
	if (!tmp_cache)
	{
		free (**cache);
		**cache = NULL;
		free (line);
		return (-1);
	}
	free (**cache);
	**cache = tmp_cache;
	*str = line;
	return (0);
}
