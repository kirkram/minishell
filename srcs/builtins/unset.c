/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:22:53 by clundber          #+#    #+#             */
/*   Updated: 2024/04/11 12:38:10 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	unset_env(t_utils *utils, char **arg, t_ms *ms)
{
	int	y;
	int	x;
	int	i;

	y = 1;
	while (arg[y])
	{
		i = 0;
		while (arg[y] && utils->envp[i])
		{
			x = 0;
			while (arg[y][x] && utils->envp[i][x])
			{
				if (arg[y][x] == utils->envp[i][x])
					x++;
				else
					break ;
				if (arg[y][x] == '\0' && utils->envp[i][x] == '=')
					remove_env(ms, i, x, y);
			}
			i++;
		}
		y++;
	}
}

void	unset_exp(t_ms *ms, char **arg, int j, int i)
{
	int	x;
	int	y;

	while (arg[++j])
	{
		i = 0;
		while (arg[j] && ms->utils->export[i])
		{
			x = 0;
			y = 0;
			while (ms->utils->export[i][y] && y < 11)
				y++;
			while (arg[j][x] && ms->utils->export[i][y])
			{
				if (arg[j][x] == ms->utils->export[i][y++])
					x++;
				else
					break ;
				if (arg[j][x] == '\0' && (ms->utils->export[i][y] == '='
					|| ms->utils->export[i][y] == '\0'))
					remove_exp(ms, i, x, y);
			}
			i++;
		}
	}
}

int	unset(t_utils *utils, char **arg, t_ms *ms)
{
	int		i;
	int		j;

	j = 0;
	i = 0;
	if (!arg || !arg[1] || !arg[1][0])
		return (0);
	unset_env(utils, arg, ms);
	unset_exp(ms, arg, j, i);
	return (0);
}

int	remove_exp(t_ms *ms, int i, int x, int y)
{
	char	**temp_arr;

	y = 0;
	x = 0;
	while (ms->utils->export[x])
		x++;
	temp_arr = malloc(sizeof(char *) * x);
	if (!temp_arr)
		malloc_check(NULL, ms);
	x = 0;
	while (ms->utils->export[x])
	{
		if (x != i)
		{
			temp_arr[y] = ft_strdup(ms->utils->export[x]);
			malloc_check(&temp_arr[y], ms);
			y++;
		}
		x++;
	}
	temp_arr[y] = NULL;
	ft_arrfree(ms->utils->export);
	ms->utils->export = temp_arr;
	return (0);
}

int	remove_env(t_ms *ms, int i, int x, int y)
{
	char	**temp_arr;

	y = 0;
	x = 0;
	while (ms->utils->envp[x])
		x++;
	temp_arr = malloc(sizeof(char *) * x);
	if (!temp_arr)
		malloc_check(NULL, ms);
	x = 0;
	while (ms->utils->envp[x])
	{
		if (x != i)
		{
			temp_arr[y] = ft_strdup(ms->utils->envp[x]);
			malloc_check(&temp_arr[y], ms);
			y++;
		}
		x++;
	}
	temp_arr[y] = NULL;
	ft_arrfree(ms->utils->envp);
	ms->utils->envp = temp_arr;
	return (0);
}