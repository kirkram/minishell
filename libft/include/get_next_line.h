/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:55:28 by clundber          #+#    #+#             */
/*   Updated: 2024/02/07 15:23:16 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdlib.h>

char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_len(const char *s);
char	*ft_strdup(const char *s);
char	*get_next_line(int fd);
int		gnlinecheck(char *str);
char	*ft_reader(char *temp, int fd);
char	*ft_rowmaker(char *temp, char *row);
char	*ft_gnlfree(char **str);
char	*rd_check(char *buffer, char *temp, int rd);

#endif
