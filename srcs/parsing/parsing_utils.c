/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 14:51:27 by clundber          #+#    #+#             */
/*   Updated: 2024/04/11 18:37:09 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	quote_status2(bool *quote, bool *dquote, char c)
{
	if (c == '\'' && (*dquote) == false)
	{
		if ((*quote) == false)
			(*quote) = true;
		else if ((*quote) == true)
			(*quote) = false;
	}
	else if (c == '\"' && (*quote) == false)
	{
		if ((*dquote) == false)
			(*dquote) = true;
		else if ((*dquote) == true)
			(*dquote) = false;
	}
}

void	quote_status(bool *quote)
{
	if ((*quote) == false)
		(*quote) = true;
	else if ((*quote) == true)
		(*quote) = false;
}

// TO BE DELETED
void	malloc_error(int err)
{
	ft_putendl_fd("minishell: Malloc failed", 2);
	exit(err);
}

int	get_token(char *str)
{
	if (!str || !str[0])
		return (0);
	else if (ft_strncmp(str, "|", 2) == 0)
		return (PIPE);
	else if (ft_strncmp(str, "<", 2) == 0)
		return (IN_FD);
	else if (ft_strncmp(str, "<<", 3) == 0)
		return (IN_HD);
	else if (ft_strncmp(str, ">", 2) == 0)
		return (OUT);
	else if (ft_strncmp(str, ">>", 3) == 0)
		return (OUT_AP);
	else if (ft_strncmp(str, "||", 3) == 0)
		return (NOT_MS);
	else if (ft_strncmp(str, "<<<", 4) == 0)
		return (NOT_MS);
	else
		return (CMD);
}

int	is_builtin(char *str)
{
	if (ft_strncmp(str, "echo", 5) == 0)
		return (8);
	else if (ft_strncmp(str, "cd", 3) == 0)
		return (8);
	else if (ft_strncmp(str, "pwd", 4) == 0)
		return (8);
	else if (ft_strncmp(str, "export", 7) == 0)
		return (8);
	else if (ft_strncmp(str, "unset", 6) == 0)
		return (8);
	else if (ft_strncmp(str, "env", 4) == 0)
		return (8);
	else if (ft_strncmp(str, "exit", 5) == 0)
		return (8);
	return (0);
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
