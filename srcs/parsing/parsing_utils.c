/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 14:51:27 by clundber          #+#    #+#             */
/*   Updated: 2024/03/20 15:58:00 by clundber         ###   ########.fr       */
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

int	syntax_err(char **array, int *err_code, int i)
{
	*err_code = 258;
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (!array[i +1] || !array[i +1][0])
		ft_putstr_fd("newline", 2);
	else
		ft_putstr_fd(array[i +1], 2);
	ft_putendl_fd("\'", 2);
	return (1);
}

void	malloc_error(int err)
{
	ft_putendl_fd("Malloc failed\n", 2);
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
	else if (ft_strncmp(str, "<<<", 3) == 0)
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
