/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:26:23 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/10 18:31:38 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


/*
env name in the format of [NAME]=
will strjoin the env name and the newstr
returns -1 on malloc fail
*/
extern int g_signal;

int	change_env_var(t_utils **utils, char *env_name, char *newstr, t_ms *ms)
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
			malloc_check(&(*utils)->envp[i], ms);
			return (0);
		}
		i ++;
	}
	tmp_arr = malloc((i + 2) * sizeof(char*));
	if (!tmp_arr)
		malloc_check(NULL, ms);
	i = 0;
	tmp_arr[0] = ft_strdup(newstr);
	malloc_check(&tmp_arr[0], ms);
	while ((*utils)->envp[i])
	{
		tmp_arr[i + 1] = ft_strdup((*utils)->envp[i]);
		if (!tmp_arr[i +1])
		{
			free_reverse(i +1, tmp_arr);
			malloc_check(NULL, ms);
		}
		free((*utils)->envp[i]);
		i ++;
	}
	tmp_arr[i + 1] = NULL;
	free((*utils)->envp);
	(*utils)->envp = tmp_arr;
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

int	update_pwd_oldpwd_env_exp(t_utils *utils, char *cwd, t_ms *ms)
{
	char	**export_args;

	export_args = malloc((3 + 1) * sizeof(char *));
	if (!export_args)
		malloc_check(NULL, ms);
	export_args[0] = ft_strdup("export");
	malloc_check(&export_args[0], ms);
	export_args[1] = ft_strjoin("OLDPWD=", cwd);
	malloc_check(&export_args[1], ms);
	getcwd(cwd, 4095);
	export_args[2] = ft_strjoin("PWD=", cwd);
	malloc_check(&export_args[2], ms);
	export_args[3] = NULL;
	export(utils, export_args, ms);
	ft_arrfree(export_args);
	return (0);
}

//should not work if you have a pipe!
//also the output of the cd command doesnt get redirected to the outfile
//i guess becuase its just stderr
// cd ./testfiles/ ; echo "hehe" will not change dir
// cd ./testfiles/folder_no_x/ | echo "hehe" will print an error as usual
int		cd_builtin(t_pipe **_pipe, t_utils *utils, int index, t_ms *ms)
{
	int		i;
	char	*home_path;
	//char	*pwd;
	char	cwd[4096]; //windows limit is 32767, usually 4096 for unix
	DIR		*directory;


	char **noio_args;
	home_path = NULL;
	//pwd = NULL;
	i = 0;
	noio_args = (_pipe)[index]->noio_args;

	while (utils->envp[i])
	{
		if (!ft_strncmp(utils->envp[i], "HOME=", 5))
			home_path = utils->envp[i] + 5;
		i ++;
	}
	getcwd(cwd, 4095);
	if (!noio_args[1] || !ft_strncmp(noio_args[1], "~", -1))
	{
		//this condition is needed to check if there is a pipe in the pipeline
		if (chdir(home_path) == -1)
		{
			directory = opendir(home_path);
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
			if (directory)
				closedir(directory);
			return (2);
		}
		update_pwd_oldpwd_env_exp(utils, cwd, ms);
	}
	else
	{
		if (chdir(noio_args[1]) == -1)
		{
			directory = opendir(noio_args[1]);
			if (access(noio_args[1], F_OK) == -1)
			{
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putstr_fd(noio_args[1], 2);
				ft_putendl_fd(": No such file or directory", 2);
			}
			else if (!directory)
			{
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putstr_fd(noio_args[1], 2);
				ft_putendl_fd(": Not a directory", 2);
			}
			else if (access(noio_args[1], X_OK) == -1)
			{
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putstr_fd(noio_args[1], 2);
				ft_putendl_fd(": Permission denied", 2);
				closedir(directory);
				return (127);
			}
			if (directory)
				closedir(directory);
			return (1);
		}
		update_pwd_oldpwd_env_exp(utils, cwd, ms);
	}
	//
	return (0);
}

int	exit_builtin(t_pipe **_pipe, t_utils *utils, int i)
{
	(void)utils;
	if (_pipe[1]) //if there is a pipe
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

int	is_only_digits_and_signs(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
		{
			if (*str != '-' && *str != '+')
				return (0);
		}
		str ++;
	}
	return (1);
}

int	change_exp_var(t_utils **utils, char *env_name, char *newstr, t_ms *ms)
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
			malloc_check(&(*utils)->export[i], ms);
			return (0);
		}
		i ++;
	}
	tmp_arr = malloc((i + 2) * sizeof(char*));
	if (!tmp_arr)
		malloc_check(NULL, ms);
	i = 0;
	tmp_arr[0] = ft_strdup(newstr);
	malloc_check(&tmp_arr[0], ms);
	while ((*utils)->export[i])
	{
		tmp_arr[i + 1] = ft_strdup((*utils)->export[i]);
		if (!tmp_arr[i + 1])
		{
			free_reverse(i +1, tmp_arr);
			malloc_check(NULL, ms);
		}
		free((*utils)->export[i]);
		i ++;
	}
	tmp_arr[i + 1] = NULL;
	free((*utils)->export);
	(*utils)->export = tmp_arr;
	return (1);
}

int	add_exp_var(t_utils **utils, char *newstr, t_ms *ms)

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
			malloc_check(&(*utils)->export[i], ms);
			return (0);
		}
		i ++;
	}
	tmp_arr = malloc((i + 2) * sizeof(char*));
	if (!tmp_arr)
		malloc_check(NULL, ms);
	i = 0;
	tmp_arr[0] = ft_strdup(newstr);
	malloc_check(&tmp_arr[0], ms);
	while ((*utils)->export[i])
	{
		tmp_arr[i + 1] = ft_strdup((*utils)->export[i]);
		if (!tmp_arr[i + 1])
		{
			free_reverse(i +1, tmp_arr);
			malloc_check(NULL, ms);
		}
		free((*utils)->export[i]);
		i ++;
	}
	tmp_arr[i + 1] = NULL;
	free((*utils)->export);
	(*utils)->export = tmp_arr;
	return (1);
}
