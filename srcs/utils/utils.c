/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 18:11:38 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/15 15:27:15 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	msg_stderr(char *cmd, char *message, int err_code)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
		ft_putstr_fd(cmd, 2);
	if (message)
		ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	return (err_code);
}

char	*jointhree(char const *s1, char const *s2, char const *s3)
{
	char	*newstr;

	if (s1 && s2 && s3)
	{
		newstr = malloc(((ft_strlen(s1) + ft_strlen(s2) \
		+ ft_strlen(s3)) + 1) * sizeof(char));
		if (newstr == NULL)
			malloc_error(1);
		ft_strlcpy(newstr, (char *)s1, -1);
		ft_strlcpy(newstr + ft_strlen(newstr), (char *)s2, -1);
		ft_strlcpy(newstr + ft_strlen(newstr), (char *)s3, -1);
		return (newstr);
	}
	return (NULL);
}

int	is_only_digits_and_signs(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
		{
			if (*str != '-' && *str != '+')
				return (0);
		}
		str ++;
	}
	return (1);
}

char	*ft_free_strjoin(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (0);
	str = malloc ((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!str)
		return (0);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = '\0';
	free (s1);
	free (s2);
	return (str);
}

int	close_if_valid_fd(int fd)
{
	if (fd > -1)
		close(fd);
	return (0);
}
