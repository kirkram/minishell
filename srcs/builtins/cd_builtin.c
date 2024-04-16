/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 18:27:21 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/16 16:31:32 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	update_pwd_oldpwd(t_utils *utils, t_ms *ms, char cwd[4096], int i)
{
	char	**export_args;
	char	oldpwd[4096];

	oldpwd[0] = '\0';
	while (utils->envp[++i])
	{
		if (ft_strncmp(utils->envp[i], "PWD=", 4) == 0)
			ft_strlcpy(oldpwd, utils->envp[i] + 4, 4096);
	}
	export_args = malloc((3 + 1) * sizeof(char *));
	if (!export_args)
		malloc_check(NULL, ms);
	export_args[0] = ft_strdup("export");
	if (!export_args[0])
		malloc_check(NULL, ms);
	export_args[1] = ft_strjoin("OLDPWD=", oldpwd);
	if (!export_args[1])
		malloc_check(NULL, ms);
	export_args[2] = ft_strjoin("PWD=", cwd);
	if (!export_args[2])
		malloc_check(NULL, ms);
	export_args[3] = NULL;
	export(utils, export_args, ms);
	ft_arrfree(export_args);
	return (0);
}

int	cd_builtin(t_pipe **_pipe, t_utils *utils, int index, t_ms *ms)
{
	char	*home_path;
	char	cwd[4096];
	int		i;

	home_path = find_home_env(utils);
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
	i = -1;
	getcwd(cwd, 4096);
	update_pwd_oldpwd(utils, ms, cwd, i);
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
