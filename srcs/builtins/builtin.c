/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:26:23 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/13 15:57:47 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


/*
env name in the format of [NAME]=
will strjoin the env name and the newstr
returns -1 on malloc fail
*/
extern int g_signal;

int	pwd(t_utils *utils)
{
	char	cwd[4096];

	(void)utils;
	if (!getcwd(cwd, -1))
		return (1);
	printf("%s\n", cwd);
	return (0);
}

int		echo_builtin(char **noio_args, t_utils *utils)
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

//max length of the mkdir name is 255 chars in this OS
int	update_pwd_oldpwd_env_exp(t_utils *utils, t_ms *ms, char cwd[4096])
{
	char	**export_args;

	export_args = malloc((3 + 1) * sizeof(char *));
	if (!export_args)
		malloc_check(NULL, ms);
	export_args[0] = ft_strdup("export");
	if (!export_args[0])
		malloc_check(NULL, ms);
	export_args[1] = ft_strjoin("OLDPWD=", cwd);
	if (!export_args[1])
		malloc_check(NULL, ms);
	getcwd(cwd, 4096);
	export_args[2] = ft_strjoin("PWD=", cwd);
	if (!export_args[2])
		malloc_check(NULL, ms);
	export_args[3] = NULL;
	export(utils, export_args, ms);
	ft_arrfree(export_args);
	return (0);
}


int	exit_builtin(t_pipe **_pipe, t_utils *utils, int i)
{
	(void)utils;
	if (_pipe[1])
		return (0);
	else if (!_pipe[i]->args[1])
	{
		ft_putendl_fd("exit", 1);
		exit(0);
	}
	else if (!is_only_digits_and_signs(_pipe[i]->args[1]))
	{
		ft_putendl_fd("exit", 1);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(_pipe[i]->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit (255);
	}
	else if (_pipe[i]->args[1] && _pipe[i]->args[2])
	{
		ft_putendl_fd("exit", 1);
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	else
	{
		ft_putendl_fd("exit", 1);
		exit(ft_atoi(_pipe[i]->args[1]));
	}
}


