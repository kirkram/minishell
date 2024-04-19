/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:19:03 by clundber          #+#    #+#             */
/*   Updated: 2024/04/19 15:33:38 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_exp(t_utils *utils, int fd)
{
	int	i;

	i = 0;
	if (!utils->export)
		return ;
	while (utils->export[i])
	{
		ft_putendl_fd(utils->export[i], fd);
		i++;
	}
}

void	sort_export(t_utils *utils)
{
	int		i;
	char	*temp;
	bool	sorted;

	sorted = false;
	while (sorted == false)
	{
		sorted = true;
		i = 0;
		while (utils->export[i])
		{
			if (utils->export[i +1] && ft_strncmp
				(utils->export[i], utils->export[i +1], -1) > 0)
			{
				sorted = false;
				temp = utils->export[i];
				utils->export[i] = utils->export[i +1];
				utils->export[i +1] = temp;
			}
			i++;
		}
	}
}

static void	add_exp_loop(char ***tmp_arr, t_utils **utils, int *i, t_ms *ms)
{
	(*tmp_arr)[(*i) + 1] = ft_strdup((*utils)->export[(*i)]);
	if (!(*tmp_arr)[(*i) + 1])
	{
		free_reverse((*i) + 1, (*tmp_arr));
		malloc_check(NULL, ms);
	}
	(*i)++;
}

/* void	replace_export(char **newstr, char **export)
{
	char **ptr;

	ptr = NULL;
	(*ptr) = (*export);

	(*export) = (*newstr);
	ft_nullfree(ptr);
} */

int	add_exp_var(t_utils **utils, char *newstr, t_ms *ms)
{
	int		i;
	char	**tmp_arr;
	char	*temp;

	temp = NULL;
	temp = ft_strjoin(newstr, "=");
	malloc_check(&temp, ms);
	i = 0;
	while ((*utils)->export[i])
	{
		if (ft_strncmp((*utils)->export[i], newstr, -1) == 0)
			return (0);
		else if(ft_strncmp((*utils)->export[i], temp, ft_strlen(temp)) == 0)
		{
			//replace_export(&newstr, &(*utils)->export[i]);
			free((*utils)->export[i]);
			(*utils)->export[i] = ft_strdup(newstr);
			ft_nullfree(&temp);
			return (0);
		}
		i ++;
	}
	tmp_arr = malloc((i + 2) * sizeof(char *));
	if (!tmp_arr)
		malloc_check(NULL, ms);
	i = 0;
	tmp_arr[0] = ft_strdup(newstr);
	malloc_check(&tmp_arr[0], ms);
	while ((*utils)->export[i])
		add_exp_loop(&tmp_arr, utils, &i, ms);
	tmp_arr[i + 1] = NULL;
	ft_arrfree((*utils)->export);
	(*utils)->export = tmp_arr;
	return (1);
}
