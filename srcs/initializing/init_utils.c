/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 18:22:46 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/16 14:19:02 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	intialize_utils(char **sys_envp, t_utils **utils, t_ms *ms)
{
	int		i;

	*(utils) = malloc(sizeof(t_utils));
	if (*utils == NULL)
		malloc_check(NULL, ms);
	i = 0;
	while (sys_envp[i])
		i ++;
	(*utils)->envp = malloc((i + 1) * sizeof(char *));
	if ((*utils)->envp == NULL)
		malloc_check(NULL, ms);
	(*utils)->export = malloc((i + 1) * sizeof(char *));
	if ((*utils)->export == NULL)
		malloc_check(NULL, ms);
	copy_utils_change_shellvars(sys_envp, utils, ms);
	(*utils)->syntax_err = false;
	(*utils)->was_prev_line_null = false;
	(*utils)->envp[i] = NULL;
	(*utils)->export[i] = NULL;
	(*utils)->err_code = 0;
	sort_export(*utils);

	char	cwd[4096];
	char	*pwd;
	i = 0;
	pwd = NULL;
	while ((*utils)->envp[i])
	{
		if (ft_strncmp("PWD=", sys_envp[i], 4) == 0)
		pwd = sys_envp[i];
		i ++;
	}
	if (!pwd)
	{
		getcwd(cwd, 4096);
		pwd = ft_strjoin("PWD=", cwd);
		change_var(&(*utils)->envp, "PWD=", pwd, ms);
		//(*utils)->export[i] = init_exp("declare -x ", cwd, ms);
		(*utils)->envp[++i] = NULL;
		free (pwd);
		//(*utils)->export[i] = NULL;
		//sort_export(*utils);
	}
}

void	copy_utils_change_shellvars(char **sys_envp, t_utils **ut, t_ms *ms)
{
	int	i;

	i = 0;
	while (sys_envp[i])
	{
		if (ft_strncmp("SHELL=", sys_envp[i], 6) == 0)
		{
			(*ut)->envp[i] = ft_strjoin("SHELL=", "minishell");
			malloc_check(&(*ut)->envp[i], ms);
			(*ut)->export[i] = init_exp("declare -x ", (*ut)->envp[i], ms);
		}
		else if (ft_strncmp("SHLVL=", sys_envp[i], 6) == 0)
		{
			(*ut)->envp[i] = shell_level(sys_envp[i], ms);
			malloc_check(&(*ut)->envp[i], ms);
			(*ut)->export[i] = init_exp("declare -x ", (*ut)->envp[i], ms);
		}
		else
		{
			(*ut)->envp[i] = ft_strdup(sys_envp[i]);
			malloc_check(&(*ut)->envp[i], ms);
			(*ut)->export[i] = init_exp("declare -x ", sys_envp[i], ms);
		}
		i ++;
	}
}

char	*shell_level(char *str, t_ms *ms)
{
	int		i;
	int		start;
	int		lvl;
	char	*temp;

	temp = NULL;
	lvl = 0;
	start = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			start = i;
			while (str[i])
				i++;
			temp = ft_substr(str, start, i - start);
			malloc_check(&temp, ms);
			lvl = ft_atoi(temp);
			ft_nullfree(&temp);
			lvl++;
			return (ft_free_strjoin(ft_substr(str, 0, start), ft_itoa(lvl)));
		}
	}
	return (ft_strdup(str));
}

static void	init_exp_variable(char **temp, int *i, char **str2, int *x)
{
	bool	equal;

	equal = false;
	while ((*str2)[(*x)])
	{
		if ((*str2)[(*x)] == '=' && equal == false)
		{
			equal = true;
			(*temp)[(*i)] = (*str2)[(*x)];
			(*temp)[(*i) + 1] = '\"';
			(*i) += 2;
			(*x)++;
		}
		else
			(*temp)[(*i)++] = (*str2)[(*x)++];
	}
	if (equal == true)
		(*temp)[(*i)++] = '\"';
	(*temp)[(*i)] = '\0';
}

char	*init_exp(char *str1, char *str2, t_ms *ms)
{
	int		i;
	int		x;
	char	*temp;

	i = 0;
	x = 0;
	temp = NULL;
	temp = malloc(sizeof(char) * (ft_strlen(str1) + ft_strlen(str2) + 3));
	malloc_check(&temp, ms);
	while (str1[i])
	{
		temp[i] = str1[i];
		i++;
	}
	init_exp_variable(&temp, &i, &str2, &x);
	return (temp);
}
