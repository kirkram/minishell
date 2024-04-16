/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shlvar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:11:23 by clundber          #+#    #+#             */
/*   Updated: 2024/04/16 15:16:48 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	shellvars(char **sys_envp, t_utils **ut, t_ms *ms, int i)
{
	if (ft_strncmp("SHELL=", sys_envp[i], 6) == 0)
	{
		(*ut)->envp[i] = ft_strjoin("SHELL=", "minishell");
		malloc_check(&(*ut)->envp[i], ms);
		(*ut)->export[i] = init_exp("declare -x ", (*ut)->envp[i], ms);
		return (0);
	}
	else if (ft_strncmp("SHLVL=", sys_envp[i], 6) == 0)
	{
		(*ut)->envp[i] = shell_level(sys_envp[i], ms, i);
		malloc_check(&(*ut)->envp[i], ms);
		(*ut)->export[i] = init_exp("declare -x ", (*ut)->envp[i], ms);
		return (1);
	}
	else
	{
		(*ut)->envp[i] = ft_strdup(sys_envp[i]);
		malloc_check(&(*ut)->envp[i], ms);
		(*ut)->export[i] = init_exp("declare -x ", sys_envp[i], ms);
		return (0);
	}
	return (0);
}

void	copy_utils_change_shellvars(char **sys_envp, t_utils **ut, t_ms *ms)
{
	int		i;
	bool	shlvl;

	shlvl = false;
	i = 0;
	while (sys_envp[i])
	{
		if (shellvars(sys_envp, ut, ms, i) == 1)
			shlvl = true;
		i ++;
	}
	(*ut)->envp[i] = NULL;
	(*ut)->export[i] = NULL;
	if (shlvl == false)
	{
		change_var(&(*ut)->envp, "SHLVL", "SHLVL=1", ms);
		change_var(&(*ut)->export, "SHLVL", "declare -x SHLVL=1", ms);
	}
}

char	*shell_level(char *str, t_ms *ms, int i)
{
	int		start;
	char	*temp;

	temp = NULL;
	start = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '=')
		{
			i++;
			start = i;
			while (str[i])
				i++;
			temp = ft_substr(str, start, i - start);
			malloc_check(&temp, ms);
			i = ft_atoi(temp);
			ft_nullfree(&temp);
			i++;
			if (i < 0 || i >= 1000)
				i = 0;
			return (ft_free_strjoin(ft_substr(str, 0, start), ft_itoa(i)));
		}
	}
	return (ft_strdup(str));
}
