/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 18:27:21 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/15 17:02:27 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	cd_builtin(t_pipe **_pipe, t_utils *utils, int index, t_ms *ms)
{
	char	*home_path;
	char	cwd[4096];

	home_path = find_home_env(utils);
	getcwd(cwd, 4096);
	if (!(_pipe)[index]->noio_args[1] || \
	!ft_strncmp((_pipe)[index]->noio_args[1], "~", 1))
	{
		if ((_pipe)[index]->noio_args[1] && \
		!ft_strncmp((_pipe)[index]->noio_args[1], "~", 1))
			home_path = jointhree \
			(home_path, \
			ft_strchr((_pipe)[index]->noio_args[1], '~') + 1, "/", ms);
		if (chdir(home_path) == -1)
			return (cd_home_chdir_fail(home_path, utils));
	}
	else
	{
		if (chdir((_pipe)[index]->noio_args[1]) == -1)
			return (cd_chdir_fail(_pipe, index));
	}
	update_pwd_oldpwd_env_exp(utils, ms, cwd);
	return (0);
}

int	cd_home_chdir_fail(char *home_path, t_utils *utils)
{
	if (access(home_path, F_OK) == -1 && !find_home_env(utils))
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putendl_fd("HOME not set", 2);
	}
	else if (access(home_path, F_OK) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(home_path, 2);
		ft_putendl_fd(": Not a directory", 2);
	}
	else if (access(home_path, X_OK) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(home_path, 2);
		ft_putendl_fd(": Permission denied", 2);
	}
	free (home_path);
	return (1);
}

int	cd_chdir_fail(t_pipe **_pipe, int index)
{
	DIR		*directory;

	directory = opendir((_pipe)[index]->noio_args[1]);
	if (access((_pipe)[index]->noio_args[1], F_OK) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd((_pipe)[index]->noio_args[1], 2);
		ft_putendl_fd(": No such file or directory", 2);
	}
	else if (!directory)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd((_pipe)[index]->noio_args[1], 2);
		ft_putendl_fd(": Not a directory", 2);
	}
	else if (access((_pipe)[index]->noio_args[1], X_OK) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd((_pipe)[index]->noio_args[1], 2);
		ft_putendl_fd(": Permission denied", 2);
	}
	if (directory)
		closedir(directory);
	return (1);
}

char	*find_home_env(t_utils *utils)
{
	char	*home_path;
	int		i;

	i = -1;
	while (utils->envp[++i])
	{
		if (!ft_strncmp(utils->envp[i], "HOME=", 5))
		{
			home_path = utils->envp[i] + 5;
			return (home_path);
		}
	}
	return (NULL);
}
