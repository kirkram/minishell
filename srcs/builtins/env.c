/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 15:38:21 by clundber          #+#    #+#             */
/*   Updated: 2024/04/11 15:18:17 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	env(t_utils *utils, char **noio_arg)
{
	int	i;

	i = 0;
	if (!utils->envp || !utils->envp[0] || noio_arg[1])
		return (0);
	while (utils->envp[i])
	{
		ft_putendl_fd(utils->envp[i], 1);
		i++;
	}
	return (0);
}

static int	change_var_loop(char ***array, char **newstr, int *i, t_ms *ms)
{
	free((*array)[(*i)]);
	(*array)[(*i)] = ft_strdup((*newstr));
	malloc_check(&(*array)[(*i)], ms);
	return (0);
}

static void	change_var_loop2(char ***array, char ***tmp_arr, int *i, t_ms *ms)
{
	(*tmp_arr)[(*i) + 1] = ft_strdup((*array)[(*i)]);
	if (!(*tmp_arr)[(*i) + 1])
	{
		free_reverse((*i) + 1, (*tmp_arr));
		malloc_check(NULL, ms);
	}
	(*i)++;
}

int	change_var(char ***array, char *env_name, char *newstr, t_ms *ms)
{
	int		i;
	char	**tmp_arr;

	i = 0;
	if (!env_name || !newstr || !(*array))
		return (1);
	while ((*array)[i])
	{
		if (ft_strncmp((*array)[i], env_name, ft_strlen(env_name)) == 0)
			return (change_var_loop(array, &newstr, &i, ms));
		i ++;
	}
	tmp_arr = malloc((i + 2) * sizeof(char *));
	if (!tmp_arr)
		malloc_check(NULL, ms);
	i = 0;
	tmp_arr[0] = ft_strdup(newstr);
	malloc_check(&tmp_arr[0], ms);
	while ((*array)[i])
		change_var_loop2(array, &tmp_arr, &i, ms);
	tmp_arr[i + 1] = NULL;
	ft_arrfree((*array));
	(*array) = tmp_arr;
	return (0);
}
