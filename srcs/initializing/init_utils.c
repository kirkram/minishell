/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 18:22:46 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/17 14:54:45 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static	void	init_look_for_pwd(char **sys_envp, t_utils **utils, t_ms *ms)
{
	int		i;
	char	cwd[4096];
	char	*pwd;

	i = 0;
	pwd = NULL;
	while ((*utils)->envp[i])
	{
		if (ft_strncmp("PWD=", (*utils)->envp[i], 4) == 0)
			pwd = sys_envp[i];
		i ++;
	}
	if (!pwd)
	{
		getcwd(cwd, 4096);
		pwd = ft_strjoin("PWD=", cwd);
		malloc_check(&pwd, ms);
		change_var(&(*utils)->envp, "PWD=", pwd, ms);
		free(pwd);
		pwd = ft_strjoin("declare -x PWD=", cwd);
		malloc_check(&pwd, ms);
		change_var(&(*utils)->export, "PWD=", pwd, ms);
		free (pwd);
	}
}

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
	(*utils)->err_code = 0;
	sort_export(*utils);
	init_look_for_pwd(sys_envp, utils, ms);
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
