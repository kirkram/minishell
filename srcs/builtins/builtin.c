/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:26:23 by klukiano          #+#    #+#             */
/*   Updated: 2024/03/08 16:57:20 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
env name in the format of [NAME]=
will strjoin the env name and the newstr
returns -1 on malloc fail
*/
int	change_env_var(t_utils **utils, char *env_name, char *newstr)
{
	int		i;
	char	**tmp_arr;

	i = 0;
	if (!env_name || !newstr || !utils)
		return (1);
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
	return (0);
}

int		echo_builtin(char **noio_args)
{
	int	i;
	int	is_newlined;

	i = 1;
	is_newlined = 1;
	if (noio_args[1] && !ft_strncmp(noio_args[1], "-n", -1))
	{
		is_newlined = 0;
		i ++;
	}
	while (noio_args[i])
	{
		if (ft_putstr_fd(noio_args[i], 1) == -1)
			return (2);
		if (noio_args[i + 1])
			if (!write(1, " ", 1))
				return (2);
		i ++;
	}
	if (is_newlined)
		if (!write(1, "\n", 1))
			return (2);
	return (0); //if write error occurs
}

int		cd_builtin(char **noio_args, t_utils *utils)
{
	int		i;
	char	*home_path;
	char	*pwd;
	char	cwd[4096]; //windows limit is 32767, usually 4096 for unix
	char	*cwd_env;

	home_path = NULL;
	pwd = NULL;
	i = 0;
	while (utils->envp[i])
	{
		if (!ft_strncmp(utils->envp[i], "HOME=", 5))
			home_path = utils->envp[i] + 5;
		else if (!ft_strncmp(utils->envp[i], "PWD=", 4))
			pwd = utils->envp[i] + 4;
		i ++;
	}

	//if malloc fails///
	getcwd(cwd, 4095);
	// printf("the cwd is now %s\n", cwd);
	if (!noio_args[1] || !ft_strncmp(noio_args[1], "~", -1))
	{
		if (chdir(home_path) == -1)
		{
			ft_putendl_fd("chdir failed", 2);
			return (2);
		}
		//easily goes into a separate function
		cwd_env = ft_strjoin("OLDPWD=", cwd);
		//if fail
		change_env_var(&utils, "OLDPWD=", cwd_env);
		free(cwd_env);
		getcwd(cwd, 4095);
		cwd_env = ft_strjoin("PWD=", cwd);
		//if fail
		change_env_var(&utils, "PWD=", cwd_env);
		free(cwd_env);
		// i = 0;
		// while (utils->envp[i])
		// {
		// 	if (!ft_strncmp(utils->envp[i], "OLDPWD=", 7))
		// 		printf("found oldpwd, its now %s\n", utils->envp[i]);
		// 	i ++;
		// }
		// getcwd(cwd, 4095);
		// printf("the cwd is now %s\n", cwd);
	}
	// else if (ft_strncmp(noio_args[1], ".", -1) || ft_strncmp(noio_args[1], "./", -1))
	// {
	//		printf("do nothing %s\n");
	// }
	else if (chdir(noio_args[1]) == -1)
	{

		//not a directory
		// permission denied
		// no such file or directory



	}



	//chdir
	//opendir, readdir, closedir:
	//if we cd add oldpwd to the env vars
	// .. and ../ are same
	// . and ./ without aythhing extra - do nothing
	// ++++++++ done +++++ cd with no args = cd ~
	//

	return (0);
}
