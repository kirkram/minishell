/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:14:40 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/26 14:16:22 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

# include <unistd.h>
# include <stdlib.h>

int		get_next_line(char **str, int fd);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlen(const char *str);
size_t	ft_strlcpy(char *dst, char *src, size_t dstsize);
void	*free_str(char *str);
size_t	find_line(char *str);
char	*ft_strjoin(char const *s1, char const *s2);
void	ft_strcpy(char *dest, char *src, char *src2);

#endif
