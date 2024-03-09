/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:26:23 by klukiano          #+#    #+#             */
/*   Updated: 2024/03/09 17:55:52 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


int	update_pwd_olppwd_env(t_utils *utils, char *cwd);

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
	return (0);
}

//should not work if you have a pipe!
//also the output of the cd command doesnt get redirected to the outfile
//i guess becuase its just stderr
// cd ./testfiles/ ; echo "hehe" will not change dir
// cd ./testfiles/folder_no_x/ | echo "hehe" will print an error as usual
int		cd_builtin(t_pipe **_pipe, t_utils *utils, int index)
{
	int		i;
	char	*home_path;
	char	*pwd;
	char	cwd[4096]; //windows limit is 32767, usually 4096 for unix

	char **noio_args;
	home_path = NULL;
	pwd = NULL;
	i = 0;
	noio_args = (_pipe)[index]->noio_args;

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
	// If home is unset only cd with no args will not work
	if (!noio_args[1] || !ft_strncmp(noio_args[1], "~", -1))
	{
		//this condition is needed to check if there is a pipe in the pipeline
		if (_pipe[0] && !_pipe[1] && chdir(home_path) == -1)
		{
			if (access(home_path, F_OK) == -1)
			{
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putendl_fd("HOME not set", 2);
			}
			else if (access(home_path, X_OK) == -1)
			{
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putstr_fd(home_path, 2);
				ft_putendl_fd(": Permission denied", 2);
			}
			//error message dependiing on why
			return (2);
		}
		if (update_pwd_olppwd_env(utils, cwd) != 0)
			return (2); //err handle
		getcwd(cwd, 4095);
		ft_putstr_fd("NoArgs. the cwd is now ", 2);
		ft_putendl_fd(cwd, 2);
	}
	else
	{
		if (_pipe[0] && !_pipe[1] && chdir(noio_args[1]) == -1)
		{
			if (access(noio_args[1], F_OK) == -1)
			{
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putstr_fd(noio_args[1], 2);
				ft_putendl_fd(": No such file or directory", 2);
			}
			else if (access(noio_args[1], X_OK) == -1)
			{
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putstr_fd(noio_args[1], 2);
				ft_putendl_fd(": Permission denied", 2);
			}
			return (2);
		}
		if (update_pwd_olppwd_env(utils, cwd) != 0)
			return (2); //err handle
		getcwd(cwd, 4095);
		ft_putstr_fd("MoreArgs. the cwd is now ", 2);
		ft_putendl_fd(cwd, 2);
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

int	update_pwd_olppwd_env(t_utils *utils, char *cwd)
{
	char	*cwd_env;

	cwd_env = ft_strjoin("OLDPWD=", cwd);
	//if fail
	change_env_var(&utils, "OLDPWD=", cwd_env);
	free(cwd_env);
	getcwd(cwd, 4095);
	cwd_env = ft_strjoin("PWD=", cwd);
	//if fail
	change_env_var(&utils, "PWD=", cwd_env);
	free(cwd_env);

	return (0);
}
