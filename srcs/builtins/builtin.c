/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:26:23 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/23 17:06:21 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	pwd(t_utils *utils)
{
	char	cwd[4096];

	(void)utils;
	if (!getcwd(cwd, 4096))
		return (1);
	printf("%s\n", cwd);
	return (0);
}

int	echo_builtin(char **noio_args, t_utils *utils)
{
	int	i;
	int	is_newlined;

	i = 1;
	is_newlined = 1;
	(void)utils;
	if (noio_args[1] && !ft_strncmp(noio_args[1], "-n", -1))
	{
		is_newlined = 0;
		i ++;
		while (noio_args[i] && !ft_strncmp(noio_args[i], "-n", -1))
			i ++;
	}
	while (noio_args[i])
	{
		ft_putstr_fd(noio_args[i], 1);
		if (noio_args[i + 1])
			write(1, " ", 1);
		i ++;
	}
	if (is_newlined)
		write(1, "\n", 1);
	return (0);
}

static long long	ft_atol(const char *str)
{
	long long		num;
	int				sign;
	long long		check;

	num = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str ++;
	if (*str == '-')
		sign = -1;
	if (*str == '+' || *str == '-')
		str ++;
	while (*str >= '0' && *str <= '9')
	{
		check = num;
		num = num * 10 + *str - 48;
		if (check > num || num < 0)
		{
			if (sign > 0)
				return (-1);
			return (0);
		}
		str ++;
	}
	return (num * sign);
}

static void	exit_builtin_success(int i, t_ms *ms)
{
	long long	exit_code;
	t_pipe		**_pipe;
	t_utils		*utils;

	_pipe = ms->pipe;
	utils = ms->utils;
	if (!_pipe[1])
		ft_putendl_fd("exit", 2);
	if (!ft_strncmp(_pipe[i]->noio_args[1], "-9223372036854775808", -1))
		free_and_exit(&_pipe, &utils, ms, 0);
	exit_code = ft_atol(_pipe[i]->noio_args[1]);
	if ((exit_code == 0 && _pipe[i]->noio_args[1][0] != '0') || \
	(exit_code == -1 && _pipe[i]->noio_args[1][0] != '-'))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(_pipe[i]->noio_args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		free_and_exit(&_pipe, &utils, ms, -42);
		exit (255);
	}
	else
		free_and_exit(&_pipe, &utils, ms, exit_code);
}

int	exit_builtin(t_pipe **_pipe, t_utils *utils, int i, t_ms *ms)
{
	(void)utils;
	if (!_pipe[1] && !_pipe[i]->noio_args[1])
	{
		ft_putendl_fd("exit", 2);
		exit(0);
	}
	else if (!is_only_digits_and_signs(_pipe[i]->noio_args[1]))
	{
		if (!_pipe[1])
			ft_putendl_fd("exit", 2);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(_pipe[i]->noio_args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit (255);
	}
	else if (_pipe[i]->noio_args[1] && _pipe[i]->noio_args[2])
	{
		if (!_pipe[1])
			ft_putendl_fd("exit", 2);
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	else
		exit_builtin_success(i, ms);
	return (1);
}
