/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 11:44:05 by clundber          #+#    #+#             */
/*   Updated: 2024/04/18 11:20:19 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	syntax_check(int *tokens, int *err_code, char **array)
{
	int	i;

	i = 0;
	while (tokens[i] != 0)
	{
		if (tokens[i] == PIPE)
		{
			if (pipe_error(tokens, err_code, i) == 1)
				return (1);
		}
		else if (tokens[i] == IN_FD && tokens[i +1] != CMD)
			return (syntax_err(array, err_code, i));
		else if (tokens[i] == IN_HD && tokens[i +1] != CMD)
			return (syntax_err(array, err_code, i));
		else if (tokens[i] == OUT && tokens[i +1] != CMD)
			return (syntax_err(array, err_code, i));
		else if (tokens[i] == OUT_AP && tokens[i +1] != CMD)
			return (syntax_err(array, err_code, i));
		else if (tokens[i] == NOT_MS)
			return (not_ms(array, err_code, i));
		i++;
	}
	return (0);
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

int	not_ms(char **array, int *err_code, int i)
{
	ft_putstr_fd("minishell: syntax error: non implemented operator: ", 2);
	ft_putendl_fd(array[i], 2);
	*err_code = 1;
	return (1);
}

int	pipe_error(int *tokens, int *err_code, int i)
{
	if (i == 0 || (i > 0 && tokens[i -1] == PIPE) || tokens[i +1] == PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		*err_code = 258;
		return (1);
	}
	if (tokens[i +1] == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		*err_code = 258;
		return (1);
	}
	return (0);
}
