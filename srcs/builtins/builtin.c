/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:26:23 by klukiano          #+#    #+#             */
/*   Updated: 2024/03/08 18:16:36 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	change_env_var(t_utils **utils, char *env_name, char *newstr)
{
	int		i;
	char	**tmp_arr;
	
	i = 0;
	if(!utils || !env_name || !newstr)
		return (0);
	while ((*utils)->envp[i])
	{
		if (ft_strncmp((*utils)->envp[i], env_name, ft_strlen(env_name)) == 0)
		{
			free((*utils)->envp[i]);
			(*utils)->envp[i] = ft_strdup(newstr);
			if ((*utils)->envp[i] == NULL)
				return (-1);
			return (0);
		}
		i ++;
	}
	tmp_arr = malloc((i + 2) * sizeof(char*));
	//mal check return (-1);
	i = 0;
	tmp_arr[0] = ft_strdup(newstr);
	//mal check return (-1);
	while ((*utils)->envp[i])
	{
		tmp_arr[i + 1] = ft_strdup((*utils)->envp[i]);
		//mal check return (-1);
		free((*utils)->envp[i]);
		i ++;
	}
	tmp_arr[i + 1] = NULL;
	free((*utils)->envp);
	(*utils)->envp = tmp_arr;
	return (1);
}

int	change_exp_var(t_utils **utils, char *env_name, char *newstr)

{
	int		i;
	char	**tmp_arr;

	i = 0;
	while ((*utils)->export[i])
	{
		if (ft_strncmp((*utils)->export[i], env_name, ft_strlen(env_name)) == 0)
		{
			free((*utils)->export[i]);
			(*utils)->export[i] = ft_strdup(newstr);
			if ((*utils)->export[i] == NULL)
				return (-1);
			return (0);
		}
		i ++;
	}
	tmp_arr = malloc((i + 2) * sizeof(char*));
	//mal check return (-1);
	i = 0;
	tmp_arr[0] = ft_strdup(newstr);
	//mal check return (-1);
	while ((*utils)->export[i])
	{
		tmp_arr[i + 1] = ft_strdup((*utils)->export[i]);
		//mal check return (-1);
		free((*utils)->export[i]);
		i ++;
	}
	tmp_arr[i + 1] = NULL;
	free((*utils)->export);
	(*utils)->export = tmp_arr;
	return (1);
}

int	add_exp_var(t_utils **utils, char *newstr)

{
	int		i;
	char	**tmp_arr;
	
	i = 0;
	while ((*utils)->export[i])
	{
		if (ft_strncmp((*utils)->export[i], newstr, -1) == 0)
		{
			free((*utils)->export[i]);
			(*utils)->export[i] = ft_strdup(newstr);
			if ((*utils)->export[i] == NULL)
				return (-1);
			return (0);
		}
		i ++;
	}
	tmp_arr = malloc((i + 2) * sizeof(char*));
	//mal check return (-1);
	i = 0;
	tmp_arr[0] = ft_strdup(newstr);
	//mal check return (-1);
	while ((*utils)->export[i])
	{
		tmp_arr[i + 1] = ft_strdup((*utils)->export[i]);
		//mal check return (-1);
		free((*utils)->export[i]);
		i ++;
	}
	tmp_arr[i + 1] = NULL;
	free((*utils)->export);
	(*utils)->export = tmp_arr;
	return (1);
}